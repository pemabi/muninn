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

struct NetResult {
    std::vector<ScoredNode> scored_nodes;
    float eval;
};

struct NetworkOutput {
    std::vector<ScoredNode> move_probs;
    float eval;
};

class Network {
public:

    static constexpr int NUM_OUPUT_POLICY = 10;

    struct NNPlanes {

    };

    static NetResult get_scored_moves(const BoardHistory& state);

    static NetworkOutput get_filtered_output(const BoardHistory& state, bool cache = false);

    static NetworkOutput get_raw_output(const BoardHistory& state, bool cache = false);

    static void gather_features(const BoardHistory& pos, NNPlanes& planes);

    static int lookup(Move move);

private:
    static std::unordered_map<Move, int> move_lookup;

    static void init_move_map();


};
