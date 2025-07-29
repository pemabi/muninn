# pragma once

extern bool cfg_verbose;
extern int cfg_max_moves;
extern int cfg_max_playouts;
extern int cfg_max_nodes;
extern int cfg_num_threads;
extern float cfg_dirichlet_epsilon;
extern float cfg_dirichlet_alpha;

class Config {
public:
    static void setup_default_config();
};
