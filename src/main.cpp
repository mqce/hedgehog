#include <Arduino.h>

#include "Drv8833.h"

namespace {

Drv8833 motors;
char line[32];
size_t lineLen = 0;

const char* skipSpace(const char* text) {
  while (*text == ' ' || *text == '\t') {
    ++text;
  }
  return text;
}

void printUsage() {
  Serial.println("L <speed>  左足 -255..255");
  Serial.println("R <speed>  右足 -255..255");
  Serial.println("B <speed>  両足 -255..255");
  Serial.println("S          ブレーキ停止");
  Serial.println("C          惰性停止");
}

void confirm(const char* label, int speed) {
  Serial.print(label);
  Serial.println(speed);
}

void handleCommand(const char* text) {
  text = skipSpace(text);
  if (*text == '\0') {
    return;
  }

  const char cmd = toupper(static_cast<unsigned char>(*text));
  const char* args = skipSpace(text + 1);

  if (cmd == 'S' && *args == '\0') {
    motors.stopAll();
    Serial.println("ブレーキ停止");
    return;
  }
  if (cmd == 'C' && *args == '\0') {
    motors.coast(MotorId::Left);
    motors.coast(MotorId::Right);
    Serial.println("惰性停止");
    return;
  }

  if (cmd == 'L' || cmd == 'R' || cmd == 'B') {
    char* end = nullptr;
    const long parsed = strtol(args, &end, 10);
    if (end == args || *skipSpace(end) != '\0') {
      printUsage();
      return;
    }
    int speed = static_cast<int>(parsed);
    if (speed > 255) {
      speed = 255;
    } else if (speed < -255) {
      speed = -255;
    }

    if (cmd == 'L') {
      motors.drive(MotorId::Left, speed);
      confirm("左足: ", speed);
    } else if (cmd == 'R') {
      motors.drive(MotorId::Right, speed);
      confirm("右足: ", speed);
    } else {
      motors.drive(MotorId::Left, speed);
      motors.drive(MotorId::Right, speed);
      confirm("両足: ", speed);
    }
    return;
  }

  printUsage();
}

}  // namespace

void setup() {
  motors.begin();
  Serial.begin(115200);
  printUsage();
}

void loop() {
  while (Serial.available() > 0) {
    const char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (lineLen > 0) {
        line[lineLen] = '\0';
        handleCommand(line);
        lineLen = 0;
      }
      continue;
    }
    if (lineLen < sizeof(line) - 1) {
      line[lineLen++] = c;
    }
  }
}
