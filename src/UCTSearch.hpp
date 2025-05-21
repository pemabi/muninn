#include "position.hpp"
#include "UCTNode.hpp"


// win probabilities in range 0-1 where 0 represents Defenders win, 1 represents Attacker win
class SearchResult {
public:
    SearchResult() = default;
    bool valid() const { return m_valid; }
    float eval() const { return m_eval; }
    static SearchResult from_eval(float eval) {
        return SearchResult(eval);
    }
    static SearchResult from_score(float board_score) {
        if (board_score > 0.0f) {
            return SearchResult(1.0f);
        } else {
            return SearchResult(0.0f);
        }
    }

private:
    explicit SearchResult(float eval)
      : m_valid(true), m_eval(eval) {}
    bool m_valid{false};
    float m_eval{0.0f};
};

class UCTSearch {
public:
    // max size of search tree in memory
    // TODO: find approx size of nodes
    static constexpr auto MAX_TREE_SIZE = 40'000'000;

    // can also have some kind of quiet flag here

    UCTSearch(BoardHistory&& bh);
    Move think(BoardHistory&& bh);
    SearchResult play_simulation(BoardHistory& bh, UCTNode* const node, int ndepth);

private:
    void dump_stats(BoardHistory& pos, UCTNode& parent);
    Move get_best_move();

    BoardHistory bh_;
    ZobristKey m_prevroot_full_key{0};
    std::unique_ptr<UCTNode> m_root;
    std::atomic<int> m_nodes{0};
    std::atomic<int> m_playouts{0};
    std::atomic<int> m_max_depth{0};

    int m_max_playouts;
    int m_max_nodes;
};
