#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include "config.h"
#include "encoder.h"
#include "pid.h"

class Motor {
public:
    Motor(const motor_cfg_t& cfg, Encoder& enc);
    void begin();
    void setVelocity(float mm_per_s);
    void update();
    float getVelocity() const { return _vel_measured; }
    void stop();
private:
    const motor_cfg_t& _cfg;
    Encoder&           _enc;
    PID_t              _pid;
    float    _vel_target;
    float    _vel_measured;
    long     _last_count;
    unsigned long _last_ms;
    void applyPWM(int pwm);
};

#endif