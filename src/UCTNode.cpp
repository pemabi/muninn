#include "UCTNode.hpp"

bool UCTNode::create_children(std::atomic<int>& node_count, const BoardHistory& state, float& eval) {
    // MT - need to add the priority checks and mutex locks

    auto net_results = Network::get_filtered_move_probs(state);

    if (net_results.move_probs.empty()) {
        return false;
    }

    // convert from Network scoring (side to move) to tree scoring (fixed Defenders = 0, Attackers = 1)
    auto net_eval = net_results.eval;
    auto to_move = state.current_pos().side_to_move();

    if (to_move == Attackers) {

    }

}
