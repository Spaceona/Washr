//
// Created by leslier on 10/1/2024.
//

#ifndef ESP32CODE_V3_IMUFUNCTIONS_H
#define ESP32CODE_V3_IMUFUNCTIONS_H

#include <Arduino.h>
#include <ICM42670P.h>

/**
 * Initializes the accelerometer with the specified range
 * @param IMU An instance of the IMU object
 * @param accelRange The accelerometer range. Ranges are 2, 4, 8, 16 from most sensitive to least
 * @param gyroRange The gyro range. Ranges are //TODO fix this
 */
void imu_init(ICM42670 &IMU, int accelRange, int gyroRange);

/**
 * The tick for the IMU state machine. Used to determine if a machine is on or not over a period of time, and debounce the signal
 * @param IMU And instance of the IMU object
 * @param accelRange The accelerometer range. Ranges are 2, 4, 8, 16 from most sensitive to least
 * @param gyroRange The gyro range. Ranges are //TODO fix this
 * @return Returns true if the tick function is in the active state (machine is on), else returns false
 */
bool imu_tick(ICM42670 &IMU, int accelRange, int gyroRange);

/**
 * Detects if the accelerometer detects motion at a specific point in time
 * @param IMU And instance of the IMU object
 * @param accelRange The accelerometer range. Ranges are 2, 4, 8, 16 from most sensitive to least
 * @param gyroRange The gyro range. Ranges are //TODO fix this
 * @return Returns true if motion is detected or false if it isn't
 */
bool motion_detected(ICM42670 &IMU, int accelRange, int gyroRange);


#endif //ESP32CODE_V3_IMUFUNCTIONS_H
