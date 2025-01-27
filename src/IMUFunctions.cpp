//Created by Robbie Leslie
//Modified by

#ifdef CUSTOM_BOARD

#include <Arduino.h>
#include <ICM42670P.h>
#include <Wire.h>
#include "IMUFunctions.h"
#include "IMUConversion.h"
#include "globals.h"

#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5

void imu_init(ICM42670 &IMU, int accelRange, int gyroRange) {

    //Setting up I2C
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    int ret;


    // Initializing the ICM42670
    ret = IMU.begin();
    if (ret != 0) {
        Serial.print("ICM42670 initialization failed: ");
        Serial.println(ret);
    }
    // Accel ODR = 100 Hz and Full Scale Range = 16G
    IMU.startAccel(1600,accelRange);
    // Gyro ODR = 100 Hz and Full Scale Range = 2000 dps
    IMU.startGyro(1600,gyroRange);
    // Wait IMU to start
    delay(100);
}




int number_seen;
bool sensor_active;
bool above_thresh;

//Again number is randomly selected should refine through testing
uint8_t debounceThresh = 30;
uint8_t debounceAboveThresh = 70; //how much higher the debouncer can go when detecting

// Setting the states for the tick function
enum IMU_States {
    Start,
    Debounce,
    Active
} IMU_State;


bool imu_tick(ICM42670 &IMU, int accelRange, int gyroRange) {

    above_thresh = motion_detected(IMU, accelRange, gyroRange);

    switch (IMU_State) { //Transitions
        case Start:
            IMU_State = Debounce;
            break;
        case Debounce:
            //Need to add more complicated state logic here
            if (number_seen >= debounceThresh) {
                IMU_State = Active;
            } else {
                IMU_State = Debounce;
            }
            break;
        case Active:
            if (number_seen >= debounceThresh) {
                IMU_State = Active;
            } else {
                IMU_State = Debounce;
            }
            break;
        default:
            IMU_State = Start;
            break;
    }

    // State logic
    switch (IMU_State) {
        case Start:
            // State logic
            sensor_active = false;
            number_seen = 0;
            detectionConfidence = number_seen;
            break;
        case Debounce:
            // State logic goes here
            sensor_active = false;
            if (above_thresh) {
                if (number_seen < debounceThresh + debounceAboveThresh) { //Bounding it to the threshold so that it wont keep adding to the debounce counter while active
                    number_seen++;
                }
            } else { //Bounding it to 0 so that having the machine idle doesn't make it impossible to detect as active
                if (number_seen != 0) {
                    number_seen--;
                }
            }
            detectionConfidence = number_seen;
            break;
        case Active:
            //State logic goes here
            sensor_active = true;
            if (above_thresh) {
                if (number_seen < debounceThresh + debounceAboveThresh) { //Bounding it to the threshold so that it wont keep adding to the debounce counter while active
                    number_seen++;
                }
            } else {
                if (number_seen != 0) {
                    number_seen--;
                }
            }
            detectionConfidence = number_seen;
            break;
        default:
            break;
    }
    return sensor_active;
}

// Super basic detection algorithm, should probably upgrade now.
// Using the accelerometer to get the data

//These are just random values. We can change them later (z accel is higher since its experiencing 9.8 g
// at least in the mounting position of usb port down like we did during the semester)
const float MPU_ACCEL_X_THRESH = 0.7;
const float MPU_ACCEL_Y_THRESH = 11;
const float MPU_ACCEL_Z_THRESH = 0.20;
const float MPU_GYRO_X_THRESH = 2;
const float MPU_GYRO_Y_THRESH = 2;
const float MPU_GYRO_Z_THRESH = 2;

// Global variables to store previous acceleration values
float prev_accel_x = 0.0;
float prev_accel_y = 0.0;
float prev_accel_z = 0.0;

// Threshold for change in acceleration
const float ACCEL_CHANGE_THRESH = 0.1; // Adjust this value as needed

bool motion_detected(ICM42670 &IMU, int accelRange, int gyroRange) {

    // Get the acceleration data
    inv_imu_sensor_event_t imu_event;

    // Get last event
    IMU.getDataFromRegisters(imu_event);

    // Convert raw accelerometer data to m/s²
    double accelX_mps2 = accelToMps2(imu_event.accel[0], accelRange);
    double accelY_mps2 = accelToMps2(imu_event.accel[1], accelRange);
    double accelZ_mps2 = accelToMps2(imu_event.accel[2], accelRange);

// Convert raw gyroscope data to rad/s
    double gyroX_rads = imu_event.gyro[0] / gyroRange;
    double gyroY_rads = imu_event.gyro[1] / gyroRange;
    double gyroZ_rads = imu_event.gyro[2] / gyroRange;

// Convert raw temperature data to degrees Celsius
    double temperature_celsius = (imu_event.temperature / 128.0) + 25.0;

    // Calculate the change in acceleration
    float delta_x = abs(accelX_mps2 - prev_accel_x);
    float delta_y = abs(accelY_mps2 - prev_accel_y);
    float delta_z = abs(accelZ_mps2 - prev_accel_z);

    // Update previous acceleration values
    prev_accel_x = accelX_mps2;
    prev_accel_y = accelY_mps2;
    prev_accel_z = accelZ_mps2;

    if (delta_x >= ACCEL_CHANGE_THRESH ||
        delta_y >= ACCEL_CHANGE_THRESH || delta_z >= ACCEL_CHANGE_THRESH) {
        return true;
    } else {
        return false;
    }
}

#endif