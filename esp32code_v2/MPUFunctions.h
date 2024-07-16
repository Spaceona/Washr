#ifndef MPU_FUNCTIONS_H
#define MPU_FUNCTIONS_H

#include <Arduino.h>
#include <Adafruit_MPU6050.h>

void mpu_init(Adafruit_MPU6050 &mpu, mpu6050_accel_range_t accel_range, mpu6050_gyro_range_t gyro_range, mpu6050_bandwidth_t bandwidth);
String get_mpu_data(Adafruit_MPU6050 &mpu);
bool mpu_tick(Adafruit_MPU6050 &mpu);
bool motion_detected(Adafruit_MPU6050 &mpu);

#endif