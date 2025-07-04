#include "position.hpp"
#include "UCTNode.hpp"
#include "parameters.hpp"
#include "training.hpp"
#include "config.hpp"


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
    static constexpr auto MAX_TREE_SIZE = 40000;

    // can also have some kind of quiet flag here

    UCTSearch(BoardHistory&& bh);
    Move search(BoardHistory&& bh);
    SearchResult play_simulation(BoardHistory& bh, UCTNode* const node, int ndepth);

    void increment_playouts();
    bool is_running();
    bool playout_limit_reached();

private:
    void dump_stats(BoardHistory& pos, UCTNode& parent);
    void dump_analysis(int64_t elapsed);
    Move get_best_move();

    BoardHistory bh_;
    ZobristKey m_prevroot_full_key{0};
    std::unique_ptr<UCTNode> m_root;
    std::atomic<int> m_nodes{0};
    std::atomic<int> m_playouts{0};
    std::atomic<int> m_max_depth{0};
    std::atomic<bool> m_run{false};
    std::chrono::steady_clock::time_point m_start_time;

    int m_max_playouts;
    int m_max_nodes;
};

class UCTWorker {
public:
    UCTWorker(const BoardHistory& bh, UCTSearch* search, UCTNode* root)
      : bh_(bh), m_search(search), m_root(root) {}
    void operator()();
private:
    const BoardHistory& bh_;
    UCTSearch* m_search;
    UCTNode* m_root;
};
