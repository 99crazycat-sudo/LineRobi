#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include "config.h"

class Encoder {
public:
    Encoder(const motor_cfg_t& cfg);
    void begin();
    void reset();
    long  count();
    float distanceMM();
    void  tick();
private:
    const motor_cfg_t& _cfg;
    volatile long _count;
};

#endif