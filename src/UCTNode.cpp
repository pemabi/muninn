#include <atomic>
#include <vector>
#include <algorithm>
#include <iterator>

#include "UCTNode.hpp"
#include "parameters.hpp"

UCTNode::UCTNode(Move move, float score, float init_eval) : m_move(move), m_score(score), m_init_eval(init_eval) {
    assert(m_score >= 0.0 && m_score <= 1.0);
}

UCTNode::~UCTNode() {
    m_children.clear();
}

int UCTNode::get_visits() const {
    return m_visits;
}

float UCTNode::get_score() const {
    return m_score;
}

void UCTNode::set_score(float score) {
    m_score = score;
}

float UCTNode::get_raw_eval(Side side) const {
    // eval without virtual loss - used in FPU
    int visits = get_visits();
    if (visits > 0) {
        double attacker_eval = get_attacker_eval();
        float score = static_cast<float>(attacker_eval / (double)visits);
        if (side == Defenders) {  // 'reverse' score for Defenders
            score = 1.0 - score;
        }
        return score;
    } else {
        // unvisited nodes inherit eval of their parent
        float eval = m_init_eval;
        if (side == Defenders) {
            eval = 1.0f - eval;
        }
        return eval;
    }
}

float UCTNode::get_eval(Side side) const {
    int virtual_loss = int{m_virtual_loss};  // to make this better for multi-threading. applies virtual loss as a 'temporary visit'
    int visits = get_visits() + virtual_loss;

    if (visits > 0) {
        auto attacker_eval = get_attacker_eval();
        if (side == Defenders) {
            attacker_eval += static_cast<double>(virtual_loss);
        }

        float score = static_cast<float>(attacker_eval / (double)visits);
        if (side == Defenders) {  // TODO: is this right? which side?
            score = 1.0f - score;
        }
        return score;
    } else {
        float eval = m_init_eval;
        if (side == Defenders) {
            eval = 1.0f - eval;
        }
        return eval;
    }
}

double UCTNode::get_attacker_eval() const {
    return m_attacker_eval;
}

Move UCTNode::get_move() const {
  return m_move;
}

bool UCTNode::create_children(std::atomic<int>& node_count, const BoardHistory& state, float& eval) {
    // MT - need to add the priority checks and mutex locks

    NetworkOutput net_results = Network::get_filtered_output(state);
    // Network need to create a struct etc with 1 probs / move pairing, and one engine eval
    // TODO: also want to change the type from auto here to that Network struct

    if (net_results.move_probs.empty()) {
        return false;
    }

    // convert from Network scoring (side to move) to tree scoring (fixed Defenders = 0, Attackers = 1)
    float net_eval = net_results.eval;
    Side to_move = state.current_pos().side_to_move();

    if (to_move == Attackers) {  // TODO: decide whether Attackers = 1.0 or Defenders = 1.0
        net_eval = 1.0f - net_eval;
    }
    eval = net_eval;

    link_node_list(node_count, net_results.move_probs, net_eval);

    return true;
}

void UCTNode::link_node_list(std::atomic<int>& node_count, std::vector<ScoredNode>& node_list, float init_eval) {
    if (node_list.empty()) {
        return;
    }

    // reverse iterate to sort probabilities in best to worst order
    std::stable_sort(rbegin(node_list), rend(node_list));

    m_children.reserve(node_list.size());
    for (const ScoredNode& node : node_list) {
        m_children.emplace_back(
            std::make_unique<UCTNode>(node.move, node.prob, init_eval)
        );
    }

    node_count += m_children.size();
    m_has_children = true;
}

UCTNode* UCTNode::uct_select(Side side, bool is_root) {
    UCTNode* best = nullptr;
    auto best_value = std::numeric_limits<double>::lowest();

    float total_visited_policy = 0.0f;
    size_t parent_visits = size_t{0};
    float net_eval = 0.0f;

    // calculating parent visits from scratch in case I add in transposition tables
    // TODO: think about transposition tables for rotational / mirrored variants?
    for (const UCTNode::node_ptr_t& child : m_children) {
        parent_visits += child->get_visits();
        if (child->get_visits() > 0) {
            total_visited_policy += child->get_score();
        } else {
            net_eval = child->get_eval(side);
        }
    }

    // First Play Urgency calc - help evaluate unvisited nodes by weighting them - helps with high branch factor games
    // Interested how this works with 'new' NN, where weights are unjustified
    // https://homepages.uc.edu/~wangyz/Publications/wang07modifications.pdf
    double numerator = std::sqrt((double)parent_visits); // numerator of PUCT bonus is sqrt of parent visits
    float fpu_reduction = 0.0f;

    if (!is_root || !cfg_noise) {
        // reduction larger when more children have been visited (more confidence in move ordering)
        fpu_reduction = cfg_fpu_reduction * std::sqrt(total_visited_policy);
    }

    // base eval for unvisited nodes is NN eval - reduction
    float fpu_eval = (cfg_fpu_dynamic_eval ? get_raw_eval(side) : net_eval) - fpu_reduction;

    for (const UCTNode::node_ptr_t& child : m_children) {
        if (!child->active()) {
            continue;
        }

        float winrate = fpu_eval;  // use fpu for unvisited nodes
        if (child->get_visits() > 0) {
            winrate = child->get_eval(side);  // otherwise use 'actual' eval based on MCTS
        }

        float nn_policy = child->get_score(); // P(s,a) - prior determined by NN
        float denom = 1.0f + child->get_visits();  // denominator is visits + 1
        float puct = cfg_puct * nn_policy * (numerator / denom);  // exploration bonus
        float value = winrate + puct; // UCT value

        // track the best child
        if (value > best_value) {
            best_value = value;
            best = child.get();
        }
    }

    assert(best != nullptr);
    return best;
}

// Not really necessary for my cases, imo. This is for more complex cases involving think time during opponent play / walking thru analysis positions
// for my intents and purposes, the new root will always be one move away from the old root
UCTNode::node_ptr_t UCTNode::find_new_root(ZobristKey prev_root_full_key, BoardHistory& new_bh) {
    UCTNode::node_ptr_t new_root = nullptr;
    std::vector<Move> moves;
    for (std::vector<Position>::reverse_iterator it  = new_bh.positions.rbegin(); it != new_bh.positions.rend(); ++it) {
        Position pos = *it;
        if (pos.full_key() == prev_root_full_key) {
            new_root = find_path(moves);  // remains nullptr if not found
            break;
        }
        moves.push_back(pos.get_move());
    }
    return new_root;
}


UCTNode::node_ptr_t UCTNode::find_path(std::vector<Move>& moves) {
    if (moves.size() == 0) {
        return nullptr;
    }
    Move move = moves.back();
    moves.pop_back();
    for (UCTNode::node_ptr_t& node : m_children) {
        if (node->get_move() == move) {
            if (moves.size() > 0) {
                return node->find_path(moves);  // little recursion here to iterate through tree
            } else {
                return std::move(node);
            }
        }
    }
    return nullptr;
}

size_t UCTNode::count_nodes() const {
    size_t node_count = size_t{0};
    if (m_has_children) {
        node_count += m_children.size();
        for (const UCTNode::node_ptr_t& child : m_children) {
            node_count += child->count_nodes();
        }
    }

    return node_count;
}

UCTNode* UCTNode::get_first_child() const {
    if (m_children.empty()) {
        return nullptr;
    }
    return m_children.front().get();
}

const std::vector<UCTNode::node_ptr_t>& UCTNode::get_children() const {
    return m_children;
}

void UCTNode::accumulate_eval(float eval) {
    atomic_add(m_attacker_eval, (double)eval);
}

void UCTNode::update(float eval) {
    m_visits++;
    accumulate_eval(eval);
}

bool UCTNode::has_children() const {
    return m_has_children;
}

bool UCTNode::active() const {
    return m_active;
}

void UCTNode::virtual_loss() {
    m_virtual_loss += VIRTUAL_LOSS_COUNT;
}

// dirichlet good for adding noise because it sums to 1
void UCTNode::dirichlet_noise(float epsilon, float alpha) {
    //alpha is our concentration parameter - spikier with low alpha, smoother with high
    // epsilon determines how much the noise will effect the probs
    size_t child_count = m_children.size();
    std::vector<float> dirichlet_vector = std::vector<float>{};

    std::gamma_distribution<float> gamma(alpha, 1.0f);
    for (size_t i = 0; i < child_count; i++) {
        dirichlet_vector.emplace_back(gamma(RandomGenerator::get_rng()));
    }

    auto sample_sum = std::accumulate(begin(dirichlet_vector), end(dirichlet_vector), 0.0f);

    if (sample_sum < std::numeric_limits<float>::min()) {
        return;
    }

    // normalize noise vector
    for (float& v: dirichlet_vector) {
        v /= sample_sum;
    }

    child_count = 0;
    for (UCTNode::node_ptr_t& child : m_children) {
        float score = child->get_score();
        float eta_a = dirichlet_vector[child_count++];
        score = score * (1 - epsilon) + epsilon * eta_a;
        child->set_score(score);
    }
}

// custom comparison between nodes
class NodeComp : public std::binary_function<UCTNode::node_ptr_t&, UCTNode::node_ptr_t&, bool> {

public:
    NodeComp(Side side) : m_side(side) {};
    bool operator()(const UCTNode::node_ptr_t& a,
                    const UCTNode::node_ptr_t& b) {
                      // if visits are not the same, sort on visits
                    if (a->get_visits() != b->get_visits()) {
                        return a->get_visits() < b->get_visits();
                    }

                    // if neither has visits, sort on prior
                    if (a->get_visits() == 0) {
                        return a->get_score() < b->get_score();
                    }

                    // if equal visits and priors, sort on engine eval
                    return a->get_eval(m_side) < b->get_eval(m_side);
                    }

private:
    Side m_side;
};

UCTNode& UCTNode::get_best_root_child(Side side) {
    assert(!m_children.empty());

    return *(std::max_element(begin(m_children), end(m_children), NodeComp(side))->get());
}

void UCTNode::sort_root_children(Side side) {
    std::stable_sort(begin(m_children), end(m_children), NodeComp(side));
    std::reverse(begin(m_children), end(m_children));
}
