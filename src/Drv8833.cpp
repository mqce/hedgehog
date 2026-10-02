#include "Drv8833.h"

#include <Arduino.h>

#include "pins.h"

namespace {

struct MotorPins {
  int in1;
  int in2;
  uint8_t ch1;
  uint8_t ch2;
};

MotorPins pinsFor(MotorId motor) {
  if (motor == MotorId::Left) {
    return {kLeftIn1, kLeftIn2, 0, 1};
  }
  return {kRightIn1, kRightIn2, 2, 3};
}

}  // namespace

void Drv8833::begin() {
  const MotorPins motors[] = {pinsFor(MotorId::Left), pinsFor(MotorId::Right)};
  for (const MotorPins& motor : motors) {
    ledcSetup(motor.ch1, kPwmHz, kPwmBits);
    ledcAttachPin(motor.in1, motor.ch1);
    ledcSetup(motor.ch2, kPwmHz, kPwmBits);
    ledcAttachPin(motor.in2, motor.ch2);
  }
  stopAll();
}

void Drv8833::drive(MotorId motor, int speed) {
  if (speed > kSpeedMax) {
    speed = kSpeedMax;
  } else if (speed < -kSpeedMax) {
    speed = -kSpeedMax;
  }

  if (speed > 0) {
    writePins(motor, speed, 0);
  } else if (speed < 0) {
    writePins(motor, 0, static_cast<uint32_t>(-speed));
  } else {
    brake(motor);
  }
}

void Drv8833::brake(MotorId motor) {
  writePins(motor, kSpeedMax, kSpeedMax);
}

void Drv8833::coast(MotorId motor) {
  writePins(motor, 0, 0);
}

void Drv8833::stopAll() {
  brake(MotorId::Left);
  brake(MotorId::Right);
}

void Drv8833::writePins(MotorId motor, uint32_t in1Duty, uint32_t in2Duty) {
  const MotorPins pins = pinsFor(motor);
  ledcWrite(pins.ch1, in1Duty);
  ledcWrite(pins.ch2, in2Duty);
}
