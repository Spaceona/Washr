//
// Created by Robbie on 9/8/24.
//

#include "IMUFunctions.h"
#include <Arduino.h>
#include <ICM42670P.h>
#include <ArduinoJson.h>
#include <Wire.h>

#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5

ICM42670 IMU(Wire,0);
int setAccelRange = 2;
int gyroRange = 2000;

int detectionConfidence = 0;

void IMUInit(){

    int ret;
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    // Initializing the ICM42670
    ret = IMU.begin();
    if (ret != 0) {
        Serial.print("ICM42670 initialization failed: ");
        Serial.println(ret);
    }
    // Accel ODR = 100 Hz and Full Scale Range = 16G
    IMU.startAccel(1600,setAccelRange);
    // Gyro ODR = 100 Hz and Full Scale Range = 2000 dps
    IMU.startGyro(1600,gyroRange);
    // Wait IMU to start
    delay(100);
}

double accelToMps2(double accelData, int accelRange) {

    switch(accelRange) {
        case 2:
            return (accelData / 16384.0) * 9.81;
        case 4:
            return (accelData / 8192.0) * 9.81;
        case 8:
            return (accelData / 4096.0) * 9.81;
        case 16:
            return (accelData / 2048.0) * 9.81;
        default:
            return -1; // Error
    }
}

String accelData(){
    inv_imu_sensor_event_t imu_event;
    IMU.getDataFromRegisters(imu_event);
    double accelX_mps2 = accelToMps2(imu_event.accel[0], setAccelRange);
    double accelY_mps2 = accelToMps2(imu_event.accel[1], setAccelRange);
    double accelZ_mps2 = accelToMps2(imu_event.accel[2], setAccelRange);

    // Create a JSON document
    JsonDocument doc;
    doc["X"] = accelX_mps2;
    doc["Y"] = accelY_mps2;
    doc["Z"] = accelZ_mps2;

    // Serialize JSON to string
    String data;
    serializeJson(doc, data);
    return data;
}

float temperature(){
    inv_imu_sensor_event_t imu_event;
    IMU.getDataFromRegisters(imu_event);
    double temperature_celsius = (imu_event.temperature / 128.0) + 25.0;
    return float(temperature_celsius);
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


bool IMU_tick() {

    above_thresh = motion_detected();
    Serial.println("Number seen: " + String(number_seen));

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
const float IMU_ACCEL_X_THRESH = 0.7;
const float IMU_ACCEL_Y_THRESH = 11;
const float IMU_ACCEL_Z_THRESH = 0.20;
const float IMU_GYRO_X_THRESH = 2;
const float IMU_GYRO_Y_THRESH = 2;
const float IMU_GYRO_Z_THRESH = 2;

// Global variables to store previous acceleration values
double prev_accel_x = 0.0;
double prev_accel_y = 0.0;
double prev_accel_z = 0.0;

// Threshold for change in acceleration
const float ACCEL_CHANGE_THRESH = 0.1; // Adjust this value as needed

bool motion_detected() {
    //Need to get the acceleration data from the IMU here
    inv_imu_sensor_event_t imu_event;

    // Get last event
    IMU.getDataFromRegisters(imu_event);

    // Convert raw accelerometer data to m/s²
    double accelX_mps2 = accelToMps2(imu_event.accel[0], setAccelRange);
    double accelY_mps2 = accelToMps2(imu_event.accel[1], setAccelRange);
    double accelZ_mps2 = accelToMps2(imu_event.accel[2], setAccelRange);


    // Calculate the change in acceleration
    float delta_x = abs(accelX_mps2 - prev_accel_x);
    float delta_y = abs(accelY_mps2 - prev_accel_y);
    float delta_z = abs(accelZ_mps2 - prev_accel_z);

    // Update previous acceleration values
    prev_accel_x = accelX_mps2;
    prev_accel_y = accelY_mps2;
    prev_accel_z = accelZ_mps2;

    Serial.println("DeltaX: " + String(delta_x));
    Serial.println("DeltaY: " + String(delta_y));
    Serial.println("DeltaZ: " + String(delta_z));

    if (delta_x >= ACCEL_CHANGE_THRESH || delta_y >= ACCEL_CHANGE_THRESH || delta_z >= ACCEL_CHANGE_THRESH) {
        return true;
    } else {
        return false;
    }
}
