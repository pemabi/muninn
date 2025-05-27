#pragma once

#include "common.hpp"
#include "position.hpp"


// IN CASE OF MT
// add virtual loss parameter? - https://liacs.leidenuniv.nl/~plaata1/papers/paper_ICAART17.pdf

class UCTNode {
    public:
        using node_ptr_t = std::unique_ptr<UCTNode>;

        explicit UCTNode(Move move, float score, float init_eval);
        UCTNode() = delete;
        ~UCTNode();
        size_t count_nodes() const;
        bool first_visit() const;
        bool has_children() const;
        bool create_children(std::atomic<int> & node_count, const BoardHistory& state, float& eval);

    private:
        void link_node_list(std::atomic<int>& node_count, std::vector<Network::scored_node>& node_list, float init_eval);

        // Move
        Move m_move;

        // UCT
        std::atomic<int16_t> m_virtual_loss{0};
        std::atomic<int> m_visits{0};

        // UCT eval
        float m_score;
        float m_init_eval;

        // Pruning?

        // Tree data
        std::atomic<bool> m_has_children{false};
        std::vector<node_ptr_t> m_children;

};
