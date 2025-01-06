// Created by Robbie Leslie
// Modified by

#ifndef ESP32CODE_V3_IMUCONVERSION_H
#define ESP32CODE_V3_IMUCONVERSION_H

/**
 * Converts acceleration data from IMU into meters per second squared
 * @param accelData The data from the accelerometer
 * @param accelRange The range that the accelerometer is set to
 * @return Returns the acceleration in m/s^2, or -1 if there is an error with the acceleration range
 */
double accelToMps2(double accelData, int accelRange);

#endif //ESP32CODE_V3_IMUCONVERSION_H
