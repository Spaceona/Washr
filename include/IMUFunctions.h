//
// Created by leslier on 10/1/2024.
//

#ifndef ESP32CODE_V3_IMUFUNCTIONS_H
#define ESP32CODE_V3_IMUFUNCTIONS_H

#include <Arduino.h>
#include <ICM42670P.h>

void imu_init(ICM42670 &IMU, int accelRange, int gyroRange);
bool imu_tick(ICM42670 &IMU, int accelRange, int gyroRange);

bool motion_detected(ICM42670 &IMU, int accelRange, int gyroRange);


#endif //ESP32CODE_V3_IMUFUNCTIONS_H
