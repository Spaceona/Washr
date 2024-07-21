#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "MPUFunctions.h"

void mpu_init(Adafruit_MPU6050 &mpu, mpu6050_accel_range_t accel_range, mpu6050_gyro_range_t gyro_range, mpu6050_bandwidth_t bandwidth){
  
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  
  mpu.setAccelerometerRange(accel_range);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(gyro_range);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(bandwidth);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
}

String get_mpu_data(Adafruit_MPU6050 &mpu) {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  char buffer[200]; // Adjust the size as needed
  snprintf(buffer, sizeof(buffer), "{\"accelerometer\":{\"z\":%f,\"y\":%f,\"x\":%f},\"gyroscope\":{\"z\":%f,\"y\":%f,\"x\":%f}}", a.acceleration.z, a.acceleration.y, a.acceleration.x, g.gyro.z, g.gyro.y, g.gyro.x);
  String dataJson = String(buffer);
  Serial.println(dataJson);
  return dataJson;
}

// Resets the MPU6050 and reconfigures it with the previous settings
void reset_mpu(Adafruit_MPU6050 &mpu) {
  // Get current settings
  mpu6050_accel_range_t accel_range = mpu.getAccelerometerRange();
  mpu6050_gyro_range_t gyro_range = mpu.getGyroRange();
  mpu6050_bandwidth_t bandwidth = mpu.getFilterBandwidth();

  // Reset the MPU6050
  mpu.reset();

  // Short delay after reset
  delay(100); // Delay to ensure MPU6050 reset process is completed

  // Set the MPU6050 to the old settings
  mpu.setAccelerometerRange(accel_range);
  mpu.setGyroRange(gyro_range);
  mpu.setFilterBandwidth(bandwidth);
}

// Setting the states for the tick function
enum MPU_States {
  Start,
  Debounce,
  Active
} MPU_State;

int number_seen;
bool sensor_active;
bool above_thresh;

//These are just random values. We can change them later (z accel is higher since its experiencing 9.8 g
// at least in the mounting position of usb port down like we did during the semester)
const uint8_t MPU_ACCEL_X_THRESH = 2;
const uint8_t MPU_ACCEL_Y_THRESH = 2;
const uint8_t MPU_ACCEL_Z_THRESH = 11;
const uint8_t MPU_GYRO_X_THRESH = 2;
const uint8_t MPU_GYRO_Y_THRESH = 2;
const uint8_t MPU_GYRO_Z_THRESH = 2;

//Again number is randomly selected should refine through testing
const uint8_t debounce_thresh = 30;

bool mpu_tick(Adafruit_MPU6050 &mpu){

  above_thresh = motion_detected(mpu);

  switch (MPU_State){
    case Start:
      MPU_State = Debounce;
      break;
    case Debounce:
      //Need to add more complicated state logic here
      if(number_seen >= debounce_thresh){
        MPU_State = Active;
      } else {
        MPU_State = Debounce;
      }
      break;
    case Active:
      if(number_seen >= debounce_thresh){
        MPU_State = Active;
      } else {
        MPU_State = Debounce;
      }
      break;
    default:
      MPU_State = Start;
      break;
  }

  // State logic
  switch (MPU_State) {
  case Start:
    // State logic
    sensor_active = false;
    number_seen = 0;
    break;
  case Debounce:
    // State logic goes here
    sensor_active = false;
    if(above_thresh){
      number_seen++;
    } else { //Bounding it to 0 so that having the machine idle doesn't make it impossible to detect as active
      if(!number_seen == 0){
        number_seen--;
      }
    }
    break;
  case Active:
    //State logic goes here
    sensor_active = true;
    if(above_thresh){
      if(number_seen <= debounce_thresh){ //Bounding it to the threshold so that it wont keep adding to the debounce counter while active
        number_seen++;
      }
    } else {
      number_seen--;
    }
    break;
  default:
    break;
  }
  return sensor_active;
}

// Super basic detection algorithm, should probably upgrade now.
// Using the accelerometer to get the data
bool motion_detected(Adafruit_MPU6050 &mpu){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  //Checking if the sensor reads 000, 000, 000 and resetting it
  if(a.acceleration.x == 0 && a.acceleration.y == 0 && a.acceleration.z == 0){
    reset_mpu(mpu);
    mpu.getEvent(&a, &g, &temp);
  }

  //Checking if the sensor is in use
  if(abs(a.acceleration.x) >= MPU_ACCEL_X_THRESH || abs(a.acceleration.y) >= MPU_ACCEL_Y_THRESH){
    return true;
  } else {
    return false;
  }
}