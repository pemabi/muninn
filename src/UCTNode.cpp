#include <vector>
#include <algorithm>
#include <iterator>

#include "UCTNode.hpp"

bool UCTNode::create_children(std::atomic<int>& node_count, const BoardHistory& state, float& eval) {
    // MT - need to add the priority checks and mutex locks

    auto net_results = Network::get_filtered_move_probs(state);
    // Network need to create a struct etc with 1 probs / move pairing, and one engine eval
    // TODO: also want to change the type from auto here to that Network struct

    if (net_results.move_probs.empty()) {
        return false;
    }

    // convert from Network scoring (side to move) to tree scoring (fixed Defenders = 0, Attackers = 1)
    auto net_eval = net_results.eval;
    auto to_move = state.current_pos().side_to_move();

    if (to_move == Attackers) {  // TODO: decide whether Attackers = 1.0 or Defenders = 1.0
        net_eval = 1.0f - net_eval;
    }
    eval = net_eval;

    /*
    I WANT TO DO ALL THIS IN NETWORK - get filtered move probs
    float legal_sum = 0.0f;
    for (auto m: net_results.move_probs) {
        legal_sum += m.prob;
    }

    // renormalizing the probs
    if (legal_sum > std::numeric_limits<float>::min()) {
        for (auto& node : net_results.move_probs) {
            node.prob /= legal_sum;
        }
    }
    */

    link_node_list(node_count, net_results.move_probs, net_eval);

    return true;
}

void UCTNode::link_node_list(std::atomic<int>& node_count, std::vector<Network::scored_node>& node_list, float init_eval) {
    if (node_list.empty()) {
        return;
    }

    // reverse iterate to sort probabilities in best to worst order
    std::stable_sort(rbegin(node_list), rend(node_list));

    m_children.reserve(node_list.size());
    for (const auto& node : node_list) {
        m_children.emplace_back(
            std::make_unique<UCTNode>(node.probs, node.move, init_eval)
        );
    }

    node_count += m_children.size();
    m_has_children = true;
}
