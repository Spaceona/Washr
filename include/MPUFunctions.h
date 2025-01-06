#ifndef MPU_FUNCTIONS_H
#define MPU_FUNCTIONS_H

#include <Arduino.h>

/**
 * Initializes the MPU with the specified ranges
 * @param mpu An instance of the mpu object
 * @param accel_range The range of the accelerometer using the mpu6050_accel_range_t type
 * @param gyro_range The range of the gyroscope using the mpu6050_gyro_range_t type
 * @param bandwidth The bandwidth of the MPU
 */
void mpu_init(Adafruit_MPU6050 &mpu, mpu6050_accel_range_t accel_range, mpu6050_gyro_range_t gyro_range, mpu6050_bandwidth_t bandwidth);

/**
 * Returns the data from the MPU in the form of a string
 * @param mpu An instance of the mpu object
 * @return the MPU data as a string in the form of "{"accelerometer":{"z":data,"y":data,"x":data},"gyroscope":{"z":data,"y":data,"x":data}}"
 */
 String get_mpu_data(Adafruit_MPU6050 &mpu);

/**
 * The tick function for the debouncer for the MPU. Detect motion over time
 * @param mpu An instance of the mpu object
 * @return Returns true if it detects motion
 */
 bool mpu_tick(Adafruit_MPU6050 &mpu);

/**
 * Checks the thresholds of the MPU to determine if it is on
 * @param mpu An instance of the mpu object
 * @return Returns true if the MPU is on
 */
bool motion_detected(Adafruit_MPU6050 &mpu);

/**
 * Resets the MPU6050 and reconfigures it with the previous settings
 * @param mpu An instance of the mpu object
 */
void reset_mpu(Adafruit_MPU6050 &mpu);

#endif