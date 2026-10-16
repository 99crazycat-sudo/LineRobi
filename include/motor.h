#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include "config.h"

class Motor {
public:
    Motor(const motor_cfg_t& cfg);
    void begin();
    void setSpeed(int speed);

private:
    const motor_cfg_t& _cfg;
};

#endif