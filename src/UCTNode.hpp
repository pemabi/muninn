#pragma once

#include "common.hpp"
#include "position.hpp"
#include "network.hpp"

// IN CASE OF MT
// add virtual loss parameter? - https://liacs.leidenuniv.nl/~plaata1/papers/paper_ICAART17.pdf

class UCTNode {
    public:
        // virtual loss adds temporary penalty to nodes that are being explored to deter other threads from exploring
        static constexpr int VIRTUAL_LOSS_COUNT = 3;

        using node_ptr_t = std::unique_ptr<UCTNode>;

        explicit UCTNode(Move move, float score, float init_eval);
        UCTNode() = delete;
        ~UCTNode();
        size_t count_nodes() const;
        UCTNode::node_ptr_t find_new_root(ZobristKey prev_root_full_key, BoardHistory& new_bh);
        UCTNode::node_ptr_t find_path(std::vector<Move>& moves);
        bool first_visit() const;
        bool has_children() const;
        bool active() const;
        bool create_children(std::atomic<int> & node_count, const BoardHistory& state, float& eval);
        Move get_move() const;

        int get_visits() const;
        float get_eval(Side side) const;
        float get_raw_eval(Side side) const;
        float get_score() const;
        void set_score(float score);
        double get_attacker_eval() const;
        void update(float eval = std::numeric_limits<float>::quiet_NaN());
        void accumulate_eval(float eval);

        void virtual_loss(void);
        void dirichlet_noise(float epsilon, float alpha);

        UCTNode* uct_select(Side side, bool is_root);
        UCTNode* get_first_child() const;
        const std::vector<node_ptr_t>& get_children() const;

        UCTNode& get_best_root_child(Side side);
        void sort_root_children(Side side);

    private:
        void link_node_list(std::atomic<int>& node_count, std::vector<ScoredNode>& node_list, float init_eval);

        // Move
        Move m_move;

        // UCT
        std::atomic<int16_t> m_virtual_loss{0};
        std::atomic<int> m_visits{0};

        // UCT eval
        float m_score;
        float m_init_eval;
        std::atomic<double>m_attacker_eval{0};
        std::atomic<bool>m_active{true};

        // Pruning?

        // Tree data
        std::atomic<bool> m_has_children{false};
        std::vector<node_ptr_t> m_children;

};
