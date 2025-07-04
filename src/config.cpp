#include "config.hpp"

bool cfg_verbose;
int cfg_max_moves;

void Config::setup_default_config() {
    cfg_verbose = true;
    cfg_max_moves = 256;
}
