//
// Created by Robbie on 9/8/24.
//

#ifndef ESP32CODE_V4_IMUFUNCTIONS_H
#define ESP32CODE_V4_IMUFUNCTIONS_H

#include <Arduino.h>

void IMUInit();

bool IMU_tick();

bool motion_detected();

double accelToMps2(double accelData, int accelRange);

String accelData();

float temperature();

#endif //ESP32CODE_V4_IMUFUNCTIONS_H
