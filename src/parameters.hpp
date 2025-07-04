#pragma once

extern float cfg_fpu_reduction;
extern bool cfg_fpu_dynamic_eval;
extern bool cfg_noise;
extern float cfg_puct;

class Parameters {
public:
    static void setup_default_parameters();

};
