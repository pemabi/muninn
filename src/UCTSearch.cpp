#include "UCTSearch.hpp"
#include "timing.hpp"

UCTSearch::UCTSearch(BoardHistory&& bh) : bh_(std::move(bh)) {
  // can add config limits for playout / node limits later

  m_root = std::make_unique<UCTNode>(moveNone(), 0.0f, 0.5f);
}

SearchResult UCTSearch::play_simulation(BoardHistory& bh, UCTNode* const node, int ndepth) {
    const Position& current = bh.current_pos();
    const Side& side = current.side_to_move();

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
        UCTNode* next = node->uct_select(side, node == m_root.get());
        Move move = next->get_move();
        bh.do_move(move);
        result = play_simulation(bh, next, ndepth+1);
    }

    if (result.valid()) {
        node->update(result.eval());
    }
    //node->virtual_loss_undo();

    return result;
}

Move UCTSearch::search(BoardHistory&& bh) {

    Time start_time;
    // look for the position in our search tree
    m_root = m_root->find_new_root(m_prevroot_full_key, bh);

    if (!m_root) {
        m_root = std::make_unique<UCTNode>(bh.current_pos().get_move(), 0.0f, 0.5f);
    }

    m_playouts = 0;
    m_max_depth = 0;
    m_nodes = m_root->count_nodes();

    bh_ = bh.shallow_clone();
    m_prevroot_full_key = bh.current_pos().full_key();

    if (!m_root->has_children()) {
        float root_eval;
        m_root->create_children(m_nodes, bh_, root_eval);
        m_root->update(root_eval);
    }

    if (cfg_noise) {
        m_root->dirichlet_noise(0.25f, 0.3f);
    }

    m_run = true;
    //int cpus = cfg_num_threads;
    // TODO: threadpool stuff
    // ThreadGroup tg(thread_pool)

    UCTWorker(bh_, this, m_root.get());

    bool keep_running = true;
    int last_update = 0;
    do {
        BoardHistory current_state = bh.shallow_clone();
        SearchResult result = play_simulation(current_state, m_root.get(), 0);
        if (result.valid()) {
            increment_playouts();
        }

        keep_running = is_running();
        keep_running &= !playout_limit_reached();
    }  while(keep_running);

    m_run = false;
    Move best_move = moveNone();
    if (!m_root->has_children()) {
        return best_move;
    }

    dump_stats(bh_, *m_root);
    Training::record(bh_, *m_root);

    // TODO: add timing stuff for analysis
    int ms_elapsed = Time::timediff_millis(start_time, Time());
    dump_analysis(ms_elapsed);

    best_move = get_best_move();
    return best_move;
}

Move UCTSearch::get_best_move() {
    Side side = bh_.current_pos().side_to_move();

    m_root->sort_root_children(side);

    Move best_move = m_root->get_first_child()->get_move();

    return best_move;
}

void UCTSearch::dump_analysis(int64_t elapsed) {
    if (!cfg_verbose) {
        return;
    }

    auto bh = bh_.shallow_clone();
    Side side = bh.current_pos().side_to_move();

    float eval = m_root->get_raw_eval(side);
    int visits = m_root->get_visits();
    int depth = m_max_depth;  // this is a slightly irrelevant stat

    // TODO: make this thread safe with custom print func
    printf("search info: depth %d | nodes %d | nps %0.f | eval %0.f | time %lld\n",
    depth, visits, 1000.0 * m_playouts / (elapsed + 1), eval, elapsed);
}

void UCTSearch::increment_playouts() {
    m_playouts++;
}

bool UCTSearch::is_running() {
    return m_run && m_nodes < MAX_TREE_SIZE;
}

bool UCTSearch::playout_limit_reached() {
    return m_playouts >= m_max_playouts || m_root->get_visits() >= m_max_nodes;
}

void UCTSearch::dump_stats(BoardHistory& pos, UCTNode& parent) {
  // print out stats for each search run
}

void UCTWorker::operator()() {
    do {
        BoardHistory bh = bh_.shallow_clone();
        auto result = m_search->play_simulation(bh, m_root, 0);
        if (result.valid()) {
            m_search->increment_playouts();
        }
    } while (m_search->is_running());
}
