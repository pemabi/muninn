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
};
