#include "motion.h"

Motion::Motion(Motor& m_left, Motor& m_right, Encoder& e_left, Encoder& e_right)
    : _mL(m_left), _mR(m_right), _eL(e_left), _eR(e_right),
      _state(IDLE), _start_ms(0), _target_time_ms(0),
      _target_dist_L(0), _target_dist_R(0) {}

void Motion::setSpeeds(float vL, float vR) {
    _mL.setVelocity(vL);
    _mR.setVelocity(vR);
}

void Motion::stop(unsigned long ms) {
    setSpeeds(0, 0);
    _state = MOVING_TIME;
    _start_ms = millis();
    _target_time_ms = ms;
}

void Motion::drive(float distance_mm, float speed_mm_s) {
    _eL.reset();
    _eR.reset();
    
    float brake_comp = 15.0f; 
    float target = distance_mm;
    if (target > 0) target = max(0.0f, distance_mm - brake_comp);
    else target = min(0.0f, distance_mm + brake_comp);

    _target_dist_L = target;
    _target_dist_R = target;

    setSpeeds(speed_mm_s, speed_mm_s);
    _state = MOVING_DIST;
}

void Motion::turn(float angle_deg, float radius_mm, float speed_mm_s) {
    _eL.reset();
    _eR.reset();

    float rad = angle_deg * PI / 180.0f;
    
    // Расстояния колес
    float dist_L = rad * (radius_mm + (WHEEL_WIDTH / 2.0f));
    float dist_R = rad * (radius_mm - (WHEEL_WIDTH / 2.0f));

    float comp = 10.0f;
    if (dist_L > 0) dist_L = max(0.0f, dist_L - comp);
    if (dist_L < 0) dist_L = min(0.0f, dist_L + comp);
    
    if (dist_R > 0) dist_R = max(0.0f, dist_R - comp);
    if (dist_R < 0) dist_R = min(0.0f, dist_R + comp);

    _target_dist_L = dist_L;
    _target_dist_R = dist_R;

    float max_dist = max(fabsf(dist_L), fabsf(dist_R));
    float vL = 0, vR = 0;
    
    if (max_dist > 1.0f) {
        vL = (dist_L / max_dist) * speed_mm_s;
        vR = (dist_R / max_dist) * speed_mm_s;
    }

    setSpeeds(vL, vR);
    _state = MOVING_ARC;
}

void Motion::update() {
    _mL.update();
    _mR.update();

    if (_state == IDLE) return;

    unsigned long now = millis();

    if (_state == MOVING_TIME) {
        if (now - _start_ms >= _target_time_ms) {
            setSpeeds(0, 0);
            _state = IDLE;
        }
        return;
    }

    if (_state == MOVING_DIST || _state == MOVING_ARC) {
        float dL = _eL.distanceMM();
        float dR = _eR.distanceMM();

        bool done_L = (fabsf(dL - _target_dist_L) <= 2.0f) || 
                      ((_target_dist_L > 0 && dL >= _target_dist_L) || (_target_dist_L < 0 && dL <= _target_dist_L));
                      
        bool done_R = (fabsf(dR - _target_dist_R) <= 2.0f) || 
                      ((_target_dist_R > 0 && dR >= _target_dist_R) || (_target_dist_R < 0 && dR <= _target_dist_R));

        if (done_L && done_R) {
            setSpeeds(0, 0);
            _state = IDLE;
        }
    }
}