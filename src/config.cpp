#include "config.hpp"

bool cfg_verbose;
int cfg_max_moves;
int cfg_max_playouts;
int cfg_max_nodes;
int cfg_num_threads;
float cfg_dirichlet_epsilon;
float cfg_dirichlet_alpha;

void Config::setup_default_config() {
    cfg_verbose = true;
    cfg_max_moves = 256;
    cfg_max_playouts = 1'073'741'823;
    cfg_max_nodes = 800;
    cfg_num_threads = 1;
    cfg_dirichlet_epsilon = 0.25f;
    cfg_dirichlet_alpha = 0.3f;
}
