#include <Arduino.h>
#include <ICM42670P.h>
#include <Wire.h>

#define LED1_PIN 6
#define LED2_PIN 10

#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5

// Instantiate an ICM42670 with LSB address set to 0
ICM42670 IMU(Wire,0);

void setup() {

    Serial.begin(115200);
    delay(2000);
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);

    //Setting up I2C
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    int ret;
    Serial.begin(115200);
    while(!Serial) {}

    // Initializing the ICM42670
    ret = IMU.begin();
    if (ret != 0) {
        Serial.print("ICM42670 initialization failed: ");
        Serial.println(ret);
    }
    // Accel ODR = 100 Hz and Full Scale Range = 16G
    IMU.startAccel(100,16);
    // Gyro ODR = 100 Hz and Full Scale Range = 2000 dps
    IMU.startGyro(100,2000);
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

    // Format data for Serial Plotter
    Serial.print("AccelX:");
    Serial.println(imu_event.accel[0]);
    Serial.print("AccelY:");
    Serial.println(imu_event.accel[1]);
    Serial.print("AccelZ:");
    Serial.println(imu_event.accel[2]);
    Serial.print("GyroX:");
    Serial.println(imu_event.gyro[0]);
    Serial.print("GyroY:");
    Serial.println(imu_event.gyro[1]);
    Serial.print("GyroZ:");
    Serial.println(imu_event.gyro[2]);
    Serial.print("Temperature:");
    Serial.println(imu_event.temperature);

    // Run @ ODR 100Hz
    delay(1000);

    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);

    IMU.getDataFromRegisters(imu_event);

    // Format data for Serial Plotter
    Serial.print("AccelX:");
    Serial.println(imu_event.accel[0]);
    Serial.print("AccelY:");
    Serial.println(imu_event.accel[1]);
    Serial.print("AccelZ:");
    Serial.println(imu_event.accel[2]);
    Serial.print("GyroX:");
    Serial.println(imu_event.gyro[0]);
    Serial.print("GyroY:");
    Serial.println(imu_event.gyro[1]);
    Serial.print("GyroZ:");
    Serial.println(imu_event.gyro[2]);
    Serial.print("Temperature:");
    Serial.println(imu_event.temperature);
    delay(1000);
}