#ifndef MOTION_H
#define MOTION_H

#include <Arduino.h>
#include "motor.h"
#include "encoder.h"
#include "config.h"

// Enum вынесен в общую область видимости, чтобы main мог его использовать
enum MotionState {
    IDLE,
    MOVING_TIME,
    MOVING_DIST,
    MOVING_ARC
};

class Motion {
public:
    Motion(Motor& m_left, Motor& m_right, Encoder& e_left, Encoder& e_right);

    void stop(unsigned long ms);
    void drive(float distance_mm, float speed_mm_s);
    void turn(float angle_deg, float radius_mm, float speed_mm_s);
    
    void update();
    
    MotionState getState() const { return _state; }
    bool isDone() const { return _state == IDLE; }

private:
    Motor&   _mL;
    Motor&   _mR;
    Encoder& _eL;
    Encoder& _eR;

    MotionState _state;
    unsigned long _start_ms;
    unsigned long _target_time_ms;
    
    float _target_dist_L;
    float _target_dist_R;

    void setSpeeds(float vL, float vR);
};

#endif