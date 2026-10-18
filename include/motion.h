#ifndef MOTION_H
#define MOTION_H

#include <Arduino.h>
#include "motor.h"
#include "encoder.h"
#include "config.h"

enum MotionState {
    IDLE,
    MOVING_TIME,
    MOVING_DIST,
    MOVING_ARC
};

class Motion {
public:
    Motion(Motor& m_left, Motor& m_right, Encoder& e_left, Encoder& e_right);

    // Остановиться на заданное время (мс)
    void stop(unsigned long ms);

    // Ехать заданное расстояние (мм) с заданной скоростью (мм/с)
    void drive(float distance_mm, float speed_mm_s);

    // Повернуть по дуге. 
    // angle_deg: угол в градусах (плюс - налево, минус - направо)
    // radius_mm: радиус дуги. 0 - на месте, WHEEL_WIDTH - одно колесо стоит.
    void turn(float angle_deg, float radius_mm, float speed_mm_s);

    // Обновление (вызывать в loop())
    void update();

    // Проверка, закончилось ли движение
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