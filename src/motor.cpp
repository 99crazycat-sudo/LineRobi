#include "motor.h"

Motor::Motor(const motor_cfg_t& cfg)
    : _cfg(cfg), _reversed(false) {}

void Motor::begin() {
    pinMode(_cfg.pin_in1, OUTPUT);
    pinMode(_cfg.pin_in2, OUTPUT);
    pinMode(_cfg.pin_en,  OUTPUT);
}

void Motor::reverse() {
    _reversed = !_reversed;
}

void Motor::setSpeed(int speed) {
    if (speed >  _cfg.max_speed) speed =  _cfg.max_speed;
    if (speed < -_cfg.max_speed) speed = -_cfg.max_speed;

    if (_cfg.invert ^ _reversed) speed = -speed;

    if (speed > 0) {
        digitalWrite(_cfg.pin_in1, HIGH);
        digitalWrite(_cfg.pin_in2, LOW);
    } else if (speed < 0) {
        digitalWrite(_cfg.pin_in1, LOW);
        digitalWrite(_cfg.pin_in2, HIGH);
    } else {
        digitalWrite(_cfg.pin_in1, LOW);
        digitalWrite(_cfg.pin_in2, LOW);
    }
    analogWrite(_cfg.pin_en, abs(speed));
}