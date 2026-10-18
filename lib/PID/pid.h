#ifndef PID_H
#define PID_H

#include <Arduino.h>
#include "config.h"

class PID_t {
public:
    PID_t(pid_cfg_t &ref_conf);
    float calc(float err, float dt);
    void reset();
private:
    pid_cfg_t &conf;
    float prev_err;
    float P, I, D;
    float integral;
    float diff;
    float out;
};

#endif