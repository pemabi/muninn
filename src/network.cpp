#include "network.hpp"

bool Network::initialized = false;
std::unordered_map<Move, int> Network::move_lookup;

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

NetworkOutput Network::get_raw_output(const BoardHistory& pos, bool cache) {
    NetworkOutput out;

    if (cache) {
        ZobristKey full_key = pos.current_pos().full_key();
        // some other stuff to build in a cache for NN output. leela xors the last 2 positions to avoid false positive lookups
        return out;
    }

    //out = get_output_internal(pos, planes);
    return out;

}

// TODO: filtering the moves out here will necessitate calculating all moves again from position - better to do this in Node where I have all the child nodes / moves
NetworkOutput Network::get_filtered_output(const BoardHistory& pos, bool cache) {

    NetworkOutput out = get_raw_output(pos, cache);

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

int Network::lookup(Move move) {
  // mask off Piece Type
  //std::cout<<"Lookup for move " << move.value() << " | Move from: " << move.from() << " | Move to: " << move.to() << " | Piece: " << move.movedPiece() << "\n";
  move = Move(move.value() | PieceNum << 14);  // ugly syntax

  return move_lookup.at(move);
}

void Network::gather_features(const BoardHistory& bh, NNPlanes& planes) {

}

NetResult Network::get_scored_moves(const BoardHistory& bh) {
    NetResult result;
    return result;
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
