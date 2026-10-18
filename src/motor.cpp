#include "motor.h"

Motor::Motor(const motor_cfg_t& cfg, Encoder& enc)
    : _cfg(cfg),
      _enc(enc),
      _pid(const_cast<pid_cfg_t&>(cfg.pid)),
      _vel_target(0),
      _vel_measured(0),
      _last_count(0),
      _last_ms(0) {}

void Motor::begin() {
    pinMode(_cfg.pin_in1, OUTPUT);
    pinMode(_cfg.pin_in2, OUTPUT);
    pinMode(_cfg.pin_en,  OUTPUT);
    _enc.reset();
    _last_count = 0;
    _last_ms = millis();
    _pid.reset();
}

void Motor::setVelocity(float mm_per_s) {
    _vel_target = mm_per_s;
}

void Motor::stop() {
    _vel_target = 0;
    _pid.reset();
    applyPWM(0);
}

void Motor::update() {
    unsigned long now = millis();
    if (now - _last_ms < VEL_UPDATE_MS) return;

    long c = _enc.count();
    long dc = c - _last_count;
    unsigned long dt_ms = now - _last_ms;
    _last_count = c;
    _last_ms = now;

    float dt = (float)dt_ms / 1000.0f;
    float dist_mm = (float)dc * MM_PER_IMP;
    _vel_measured = dist_mm / dt;

    float err = _vel_target - _vel_measured;
    float out = _pid.calc(err, dt);

    float base = 0.0f;
    if (_vel_target > 0.1f)  base =  (float)_cfg.dead_zone;
    if (_vel_target < -0.1f) base = -(float)_cfg.dead_zone;

    float pwm = base + out;

    if (pwm >  _cfg.max_speed) pwm =  _cfg.max_speed;
    if (pwm < -_cfg.max_speed) pwm = -_cfg.max_speed;

    applyPWM((int)pwm);
}

void Motor::applyPWM(int pwm) {
    if (pwm >  _cfg.max_speed) pwm =  _cfg.max_speed;
    if (pwm < -_cfg.max_speed) pwm = -_cfg.max_speed;

    if (_cfg.invert) pwm = -pwm;

    if (pwm > 0) {
        digitalWrite(_cfg.pin_in1, HIGH);
        digitalWrite(_cfg.pin_in2, LOW);
    } else if (pwm < 0) {
        digitalWrite(_cfg.pin_in1, LOW);
        digitalWrite(_cfg.pin_in2, HIGH);
    } else {
        digitalWrite(_cfg.pin_in1, LOW);
        digitalWrite(_cfg.pin_in2, LOW);
    }
    analogWrite(_cfg.pin_en, abs(pwm));
}