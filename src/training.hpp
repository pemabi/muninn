#pragma once

#include "common.hpp"
#include "UCTNode.hpp"

class TimeStep {
public:
    Network::NNPlanes planes;
    std::vector<float> probabilities;
    Side to_move;
    float net_winrate;
    float root_uct_winrate;
    float child_uct_winrate;
    int best_move_visits;
};

// TODO: is this overkill
class OutputChunker {

};

class Training {
public:
    static void clear_training();
    static void dump_training(int game_score, OutputChunker& outchunker);
    static void dump_stats(const std::string& out_filename);
    static void record(const BoardHistory& state, UCTNode& node);

private:
    static void dump_stats(OutputChunker& outchunker);
    static std::vector<TimeStep> m_data;
};
