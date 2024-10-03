#include <Arduino.h>
#include <ICM42670P.h>
#include <Wire.h>
#include "IMUConversion.h"

#define LED1_PIN 6
#define LED2_PIN 10

#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5

// Instantiate an ICM42670 with LSB address set to 0
ICM42670 IMU(Wire,0);
int accelRange = 2;
int gyroRange = 2000;

void setup() {

    Serial.begin(115200);
    delay(2000);
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);

    //Setting up I2C
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    int ret;
    Serial.begin(115200);
    while(!Serial) {

    }

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

void loop() {

    Serial.println("Hello World");
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);


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

// Print converted values
    Serial.print("AccelX (m/s^2): ");
    Serial.println(accelX_mps2);
    Serial.print("AccelY (m/s^2): ");
    Serial.println(accelY_mps2);
    Serial.print("AccelZ (m/s^2): ");
    Serial.println(accelZ_mps2);
    Serial.print("GyroX (rad/s): ");
    Serial.println(gyroX_rads);
    Serial.print("GyroY (rad/s): ");
    Serial.println(gyroY_rads);
    Serial.print("GyroZ (rad/s): ");
    Serial.println(gyroZ_rads);
    Serial.print("Temperature (C): ");
    Serial.println(temperature_celsius);


    delay(50);

    //digitalWrite(LED1_PIN, LOW);
    //digitalWrite(LED2_PIN, LOW);


    //delay(50);
}