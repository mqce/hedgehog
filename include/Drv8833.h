#pragma once

#include <stdint.h>

enum class MotorId {
  Left,
  Right,
};

class Drv8833 {
 public:
  void begin();
  void drive(MotorId motor, int speed);
  void brake(MotorId motor);
  void coast(MotorId motor);
  void stopAll();

 private:
  static constexpr int kSpeedMax = 255;
  static constexpr uint32_t kPwmHz = 20000;
  static constexpr uint8_t kPwmBits = 8;

  void writePins(MotorId motor, uint32_t in1Duty, uint32_t in2Duty);
};
