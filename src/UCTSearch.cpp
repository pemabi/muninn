#include "UCTSearch.hpp"


UCTSearch::UCTSearch(BoardHistory&& bh) : bh_(std::move(bh)) {
  // can add config limits for playout / node limits later

  m_root = std::make_unique<UCTNode>(moveNone, 0.0f, 0.5f);
}

SearchResult UCTSearch::play_simulation(BoardHistory& bh, UCTNode* const node, int ndepth) {
    const auto& current = bh.current_pos();
    const auto& side = current.side_to_move();

    auto result = SearchResult{};

    // in case of MT
    //node->virtual_loss();

    if (ndepth > m_max_depth) {
        m_max_depth = ndepth;
    }

    if (!node->has_children()) {    // if the node has no children, either a leaf node or no possible moves
        if (!MoveList(current).size()) {    // if no possible moves, return result as opponent win
            float score = side == Attackers ? -1.0 : 1.0;  // if it is Attackers turn, returns -1. Defenders returns 1.
            result = SearchResult::from_score(score);
        }

        else if (m_nodes < MAX_TREE_SIZE) {   // otherwise, this is a leaf node so expand node and return result as engine eval (initial scoring)
            float eval;
            bool success = node->create_children(m_nodes, bh, eval);
            if (success) {
                result = SearchResult::from_eval(eval);
            }
        }
    }

    if (node->has_children() && !result.valid()) {  // if node has children, and there is no valid result, recursion thru tree until we get a result
        auto next = node->uct_select(side, node == m_root.get());
        auto move = next->get_move();
        bh.do_move(move);
        result = play_simulation(bh, next, ndepth+1);
    }

    if (result.valid()) {
        node->update(result.eval());
    }
    //node->virtual_loss_undo();

    return result;
}
