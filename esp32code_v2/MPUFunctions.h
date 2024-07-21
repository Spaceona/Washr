#ifndef MPU_FUNCTIONS_H
#define MPU_FUNCTIONS_H

#include <Arduino.h>
#include <Adafruit_MPU6050.h>

//Initializes the MPU
void mpu_init(Adafruit_MPU6050 &mpu, mpu6050_accel_range_t accel_range, mpu6050_gyro_range_t gyro_range, mpu6050_bandwidth_t bandwidth);

//Gets the MPU data as a string in the form of "{"accelerometer":{"z":data,"y":data,"x":data},"gyroscope":{"z":data,"y":data,"x":data}}"
String get_mpu_data(Adafruit_MPU6050 &mpu);

//The tick function for the debouncer for the mpu
bool mpu_tick(Adafruit_MPU6050 &mpu);

//Checks the thresholds of the mpu to determine if it is on
bool motion_detected(Adafruit_MPU6050 &mpu);

// Resets the MPU6050 and reconfigures it with the previous settings
void reset_mpu(Adafruit_MPU6050 &mpu);

#endif