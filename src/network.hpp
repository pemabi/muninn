# pragma once

#include <unordered_map>

#include "position.hpp"

struct ScoredNode {
    float prob;
    Move move;

    // so that this can work with stable sort
    bool operator<(const ScoredNode& other) const {
        return prob < other.prob;
    }
};

struct NetworkOutput {
    std::vector<ScoredNode> move_probs;
    float eval;
};

class Network {
public:
    // 4 (attackers + defenders + king + reps) * 8 = 32
    // TODO: should I add an all Defenders planes?
    // TODO: should I add a plane for the Throne square? Empty Throne Square?
    // TODO: should change the order of the planes based on turn? ie first plane always includes all pieces
    // 1 side
    // 1 unused plane of 1s for padding

    static constexpr int T_HISTORY = 8;
    static constexpr int T_PLANES = 4;
    static constexpr int INPUT_CHANNELS = T_PLANES * T_HISTORY + 2;

    static constexpr int NUM_OUPUT_POLICY = 1296;
    static constexpr int NUM_OUTPUT_VALUE = 1;
    static constexpr int NUM_VALUE_CHANNELS = 128;
    static constexpr int NUM_VALUE_INPUT_PLANES = 32;
    static constexpr int NUM_POLICY_INPUT_PLANES = 32;

    // Winograd F(4x4, 3x3)
    static constexpr int WINOGRAD_M = 4; // output tile size
    static constexpr int WINOGRAD_R = 3; // filter size
    static constexpr int WINOGRAD_ALPHA = WINOGRAD_M + WINOGRAD_R - 1; // transformed space size
    static constexpr int WINOGRAD_WTILES = 9 / WINOGRAD_M + (9 % WINOGRAD_M != 0); // num tiles p/ dim (ceiling division)
    static constexpr int WINOGRAD_TILE = WINOGRAD_ALPHA * WINOGRAD_ALPHA; // elements in winograd space
    static constexpr int WINOGRAD_P = WINOGRAD_WTILES * WINOGRAD_WTILES; // total tiles needed to cover board
    static constexpr float SQ2 = 1.4142135623730951f; // Square root of 2

    using BoardPlane = std::bitset<9 * 9>;
    using net_t = float;

    struct NNPlanes {
        std::array<BoardPlane, INPUT_CHANNELS - 3> bit;
    };

    static void initialize();

    static NetworkOutput get_network_output(const BoardHistory& state, bool cache = false);
    static NetworkOutput get_network_output_internal(const BoardHistory& state, NNPlanes& planes);
    static NetworkOutput get_filtered_output_fake(const BoardHistory& state, bool cache = false);

    static void gather_features(const BoardHistory& pos, NNPlanes& planes);
    static int lookup(Move move);

private:
    static bool initialized;
    static std::unordered_map<Move, int> move_lookup;

    static void init_move_map();

    static void softmax(const std::vector<float>& input, std::vector<float>& output, float temperature = 1.0f);
    static void winograd_convolve(const int outputs,
                                            const std::vector<float>& input,
                                            const std::vector<float>& U,
                                            std::vector<float>& V,
                                            std::vector<float>& M,
                                            std::vector<float>& output);
    static void winograd_transform_in(const std::vector<float>& in,
                                      std::vector<float>& V, const int C);
    static void forward_cpu(std::vector<float>& input, std::vector<float>& output_pol, std::vector<float>& output_val);


};
