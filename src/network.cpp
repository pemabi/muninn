#include <Accelerate/Accelerate.h>

#include "network.hpp"
#include "config.hpp"

bool Network::initialized = false;
std::unordered_map<Move, int> Network::move_lookup;

// Input + residual block tower
static std::vector<std::vector<float>> conv_weights;
static std::vector<std::vector<float>> conv_biases;
static std::vector<std::vector<float>> batchnorm_means;
static std::vector<std::vector<float>> batchnorm_stddivs;

// Policy head
static std::vector<float> conv_pol_w;
static std::vector<float> conv_pol_b;

static std::array<float, Network::NUM_VALUE_CHANNELS> ip2_val_w;
static std::array<float, 1> ip2_val_b;

void Network::initialize() {
    if (initialized) return;
    initialized = true;

    std::cout<<"initializing network...\n";
    init_move_map();
}

void Network::init_move_map() {
    // initialise the move map that can map output vector indices to moves
    std::cout<<"initializing move map...\n";

    std::vector<Move> moves;

    for (Square sq = SQA1; sq <= SQI9; ++sq) {
        Bitboard bb = get_moves_unmasked(sq, EMPTY_BB);

        while (bb) {
            // note that piece type is NOT included in move, and must be inferred later to work with current move routine
            // adding piece type would add way too much info for the net to infer and make the output space too large
            moves.push_back(encodeMove(PieceNum, sq, bb.bitscan_pop_forward()));  // TODO: need to extract the value of the move for hash func to work?
        }
    }

    for (size_t i = 0; i < moves.size(); ++i) {
        move_lookup[moves[i]] = i;
    }
}

void Network::winograd_convolve_in(const std::vector<float>& in, std::vector<float>& V, const int C) {
    constexpr auto Wpad = 2 + WINO
}

// https://arxiv.org/pdf/1509.09308
void Network::winograd_convolve(const int outputs,
                                const std::vector<float>& input,
                                const std::vector<float>& U,
                                std::vector<float>& V,
                                std::vector<float>& M,
                                std::vector<float>& output) {
    constexpr unsigned int filter_len = WINOGRAD_TILE;
    const auto input_channels = U.size() / (outputs * filter_len);
}

void Network::forward_cpu(std::vector<float>& input, std::vector<float>& output_pol, std::vector<float>& output_val) {
    // input conv
    constexpr int P = WINOGRAD_P;
    // calculate output channels
    const int output_channels = conv_biases[0].size();
    // input channels is max inp channels of convolution
    const size_t input_channels = std::max(static_cast<size_t>(output_channels), static_cast<size_t>(INPUT_CHANNELS));
    std::vector<float> conv_out(output_channels * 81);

    std::vector<float> V(WINOGRAD_TILE * input_channels * P);
    std::vector<float> M(WINOGRAD_TILE * output_channels * P);

    winograd_convolve(output_channels, input, conv_weights[0], V, M, conv_out);
}

void Network::softmax(const std::vector<float>& input, std::vector<float>& output, float temperature) {
    assert(&input != &output);

    float alpha = *std::max_element(begin(input), begin(input) + output.size());

    alpha /= temperature;

    float denom = 0.0f;
    std::vector<float> helper(output.size());
    for (size_t i = size_t{0}; i < output.size(); i++) {
        float val = std::exp((input[i] / temperature) - alpha);
        helper[i] = val;
        denom += val;
    }
    for (size_t i = size_t{0}; i < output.size(); i++) {
        output[i] = helper[i] / denom;
    }
}

template<unsigned int inputs, unsigned int outputs, size_t W, size_t B>
void inner_product(const std::vector<float>& input,
                  const std::array<float,
                  W>& weights,
                  const std::array<float, B>& biases,
                  std::vector<float>& output) {
    assert(B == outputs);

    //         (matrix order,  no transpose, dim M  , dim K , alpha, matrix ptr, lead dim, inp ptr, stride, beta, out ptr, stride
    cblas_sgemv(CblasRowMajor, CblasNoTrans, outputs, inputs, 1.0, &weights[0], inputs, &input[0], 1, 0.0f, &output[0], 1);
    // output[i] = Σ(weights[i][j] * input[j]) for j = 0 to inputs-1

    float lambda_ReLU = [](float val) { return (val > 0.0f) ? val : 0.0f; };

    for (unsigned int o = 0; o < outputs; 0++) {
        float val = biases[o] + output[o];
        if (outputs == Network::NUM_VALUE_CHANNELS) {
            val = lambda_ReLU(val);
        }
        output[o] = val;
    }
}

NetworkOutput Network::get_network_output(const BoardHistory& pos, bool cache) {

    NetworkOutput out;

    /*

    TODO: CACHE STUFF

    ZobristKey full_key = pos.current_pos().full_key();

    int history_count = pos.positions.size();

    for (int i = history_count - 2; i >= std::max(0, history_count - T_HISTORY); i--) {
        full_key *= 31; // this is crucial to encoding the order of positions - XOR has commutative and associative properties
        full_key ^= pos.positions[i].full_key();
    }

    if (!skip_chache) {
        if (NNCache::get_NNCache().lookup(full_key, result)) {
            return result;
        }
    }

    */

    NNPlanes planes;
    gather_features(pos, planes);
    out = get_network_output_internal(pos, planes);

    // NNCache::get_NNCache().instert(full_key, out);

    return out;

    float legal_sum = 0.0f;
    for (ScoredNode m: out.move_probs) {
        legal_sum += m.prob;
    }

    // normalizing the move probs
    if (legal_sum > std::numeric_limits<float>::min()) {
      for (auto& node : out.move_probs) {
          node.prob /= legal_sum;
      }
    } else {  // if valid move probs are negligible, sets them all equal so they sum to 1
        float uniform_prob = 1.0f / out.move_probs.size();
        for (ScoredNode& node : out.move_probs) {
            node.prob = uniform_prob;
        }
    }

    return out;
}

NetworkOutput Network::get_network_output_internal(const BoardHistory& bh, NNPlanes& planes) {
    assert(INPUT_CHANNELS == planes.bit.size());
    constexpr int width = 9;
    constexpr int height = 9;

    const auto convolve_channels = conv_pol_w.size() / conv_pol_b.size();
    std::vector<net_t> input_data;
    std::vector<net_t> output_data(convolve_channels * width * height);
    std::vector<float> value_data(Network::NUM_VALUE_INPUT_PLANES * width * height);
    std::vector<float> policy_data(NUM_OUPUT_POLICY);
    std::vector<float> softmax_data(NUM_OUPUT_POLICY);
    std::vector<float> winrate_data(Network::NUM_VALUE_CHANNELS);
    std::vector<float> winrate_out(1);
    // data layout is input_data[(c * height + h) * width + w]
    input_data.reserve(INPUT_CHANNELS * width * height);
    for (int c = 0; c < INPUT_CHANNELS; ++c) {
        for (int i = 0; i < width * height; ++i) {
            input_data.emplace_back(net_t(planes.bit[c][i]));
        }
    }
    assert(input_data.size() == INPUT_CHANNELS * width * height);

    forward_cpu(input_data, policy_data, value_data);

    // policy head
    softmax(policy_data, softmax_data, cfg_softmax_temp);
    std::vector<float>& outputs = softmax_data;

    // value head
    inner_product<NUM_VALUE_CHANNELS, 1>(value_data, ip2_val_w, ip2_val_b, winrate_out);
    auto winrate_sig = (1.0f + std::tanh(winrate_out[0])) / 2.0f;

    MoveList moves(bh.current_pos());
    std::vector<ScoredNode> output;
    for (Move move : moves) {
        output.emplace_back(ScoredNode{outputs[lookup(move)], move});
    }

    return NetworkOutput{output, winrate_sig};
}

int Network::lookup(Move move) {
  // mask off Piece Type
  //std::cout<<"Lookup for move " << move.value() << " | Move from: " << move.from() << " | Move to: " << move.to() << " | Piece: " << move.movedPiece() << "\n";
  move = Move(move.value() | PieceNum << 14);  // ugly syntax

  return move_lookup.at(move);
}

template<PieceType Pt>
void add_pieces(const Position* pos, Side side, Network::NNPlanes& planes, int plane_idx) {
    const Square* squares = pos->squares<Pt>(side);
    while (*squares != SQ_NONE) {
        Square sq = *squares++;
        planes.bit[plane_idx][sq] = true;
    }
}

void Network::gather_features(const BoardHistory& bh, NNPlanes& planes) {
    Side us = bh.current_pos().side_to_move();
    Side them = ~us;
    const Position* pos = &bh.current_pos();

    int kFeatureBase = T_HISTORY * T_PLANES;
    if (us == Attackers) planes.bit[kFeatureBase + 0].set();
    planes.bit[kFeatureBase + 1].set();  // last plane of 1s for padding

    int bh_idx = bh.positions.size() - 1;

    Side side = pos->side_to_move();

    for (int i = 0; i < std::min(T_HISTORY, bh_idx + 1); ++i) {
        pos = &bh.positions[bh_idx - i];

        add_pieces<Attacker>(pos, side, planes, i * T_PLANES + 0);
        add_pieces<Defender>(pos, side, planes, i * T_PLANES + 1);
        add_pieces<King    >(pos, side, planes, i * T_PLANES + 2);

        int repetitions = pos->repetitions_count();
        if (repetitions >= 1) planes.bit[i * T_PLANES + 3].set();
    }
}

NetworkOutput Network::get_filtered_output_fake(const BoardHistory& bh, bool cache) {
    // implement uniform prob distribtion
    NetworkOutput output;

    MoveList moves(bh.current_pos());

    float uniform_prob = 1.0f / moves.size();  // uniform probability distribution

    for (const auto& move : moves) {
        output.move_probs.emplace_back(ScoredNode{uniform_prob, move});
    }

    std::uniform_real_distribution<float> eval_dist(0.45f, 0.55f);
    output.eval = eval_dist(RandomGenerator::get_rng());

    return output;
}
