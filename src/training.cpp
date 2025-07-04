#include "training.hpp"

std::vector<TimeStep> Training::m_data{};

void Training::clear_training() {
    Training::m_data.clear();
}

void Training::record(const BoardHistory& state, UCTNode& root) {
    auto step = TimeStep{};
    step.to_move = state.current_pos().side_to_move();
    step.planes = Network::NNPlanes{};
    Network::gather_features(state, step.planes);

    NetResult result = Network::get_scored_moves(state);
    step.net_winrate = result.eval;

    const auto& best_node = root.get_best_root_child(step.to_move);
    step.root_uct_winrate = root.get_eval(step.to_move);
    step.child_uct_winrate = best_node.get_eval(step.to_move);
    step.best_move_visits = best_node.get_visits();

    step.probabilities.resize(Network::NUM_OUPUT_POLICY); // resizing vector to fit the output space

    auto sum_visits = 0.0;
    for (const auto& child : root.get_children()) {
        sum_visits += child->get_visits();
    }

    // if the position is terminal, don't construct probabilities
    if (sum_visits <= 0.0) {
        return;
    }

    for (const auto& child : root.get_children()) {
        auto prob = static_cast<float>(child->get_visits() / sum_visits);
        auto move = child->get_move();
        step.probabilities[Network::lookup(move)] = prob;
    }

    m_data.emplace_back(step);
}

void Training::dump_training(int game_score, OutputChunker& outchunk) {

}

void Training::dump_stats(const std::string& out_filename) {

}
