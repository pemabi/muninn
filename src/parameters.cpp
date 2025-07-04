#include "parameters.hpp"

float cfg_fpu_reduction;
bool cfg_fpu_dynamic_eval;
bool cfg_noise;
float cfg_puct;

void Parameters::setup_default_parameters() {
    cfg_fpu_reduction = 0.1f;
    cfg_fpu_dynamic_eval = true;
    cfg_noise = false;
    cfg_puct = 0.6f;
}
