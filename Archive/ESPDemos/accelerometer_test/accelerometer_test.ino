// Basic demo for accelerometer readings from Adafruit MPU6050

// ESP32 Guide: https://RandomNerdTutorials.com/esp32-mpu-6050-accelerometer-gyroscope-arduino/
// ESP8266 Guide: https://RandomNerdTutorials.com/esp8266-nodemcu-mpu-6050-accelerometer-gyroscope-arduino/
// Arduino Guide: https://RandomNerdTutorials.com/arduino-mpu-6050-accelerometer-gyroscope/

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

// Setting up the LED
int led_1 = D8;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
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
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
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

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
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

  pinMode(led_1, OUTPUT);

  Serial.println("");
  delay(100);
}

// Setting up the timers for the mpu debounce state machine
int mpu_timer1 = 0;
int mpu_timer2 = millis();
int mpu_period = 50;
  
bool in_use;

void loop() {
  

  
  // delay(500);

  mpu_timer2 = millis();

  if((mpu_timer2 - mpu_timer1) >= mpu_period){

    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    /* Print out the values */
    // Serial.print("Acceleration X: ");
    // Serial.print(a.acceleration.x);
    // Serial.print(", Y: ");
    // Serial.print(a.acceleration.y);
    // Serial.print(", Z: ");
    // Serial.print(a.acceleration.z);
    // Serial.println(" m/s^2");

    // Serial.print("Rotation X: ");
    // Serial.print(g.gyro.x);
    // Serial.print(", Y: ");
    // Serial.print(g.gyro.y);
    // Serial.print(", Z: ");
    // Serial.print(g.gyro.z);
    // Serial.println(" rad/s");

    // Serial.print("Temperature: ");
    // Serial.print(temp.temperature);
    // Serial.println(" degC");

    // Serial.println("");

    in_use = mpu_tick(mpu);
    Serial.println("Machine status: ");
    Serial.println(in_use);
    mpu_timer1 = mpu_timer2;
  }
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
const float MPU_ACCEL_X_THRESH = 9.7;
const float MPU_ACCEL_Y_THRESH = 0.1;
const float MPU_ACCEL_Z_THRESH = 0.73;
const float MPU_GYRO_X_THRESH = 2;
const float MPU_GYRO_Y_THRESH = 2;
const float MPU_GYRO_Z_THRESH = 2;

//Again number is randomly selected should refine through testing
const uint8_t debounce_thresh = 30;
const uint8_t debounce_above_thresh = 70; //how much higher the debouncer can go when detecting

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
      if(number_seen <= debounce_thresh+debounce_above_thresh){ //Bounding it to the threshold so that it wont keep adding to the debounce counter while active
        number_seen++;
      }
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
      digitalWrite(led_1, HIGH);
      if(number_seen <= debounce_thresh+debounce_above_thresh){ //Bounding it to the threshold so that it wont keep adding to the debounce counter while active
        number_seen++;
      }
    } else {
      digitalWrite(led_1, LOW);
      if(!number_seen == 0){
        number_seen--;
      }
    }
    break;
  default:
    break;
  }
  Serial.print("Number seen: ");
  Serial.println(number_seen);
  return sensor_active;
}

// Super basic detection algorithm, should probably upgrade now.
// Using the accelerometer to get the data
// Global variables to store previous acceleration values
float prev_accel_x = 0.0;
float prev_accel_y = 0.0;
float prev_accel_z = 0.0;

// Threshold for change in acceleration
const float ACCEL_CHANGE_THRESH = 0.03; // Adjust this value as needed

bool motion_detected(Adafruit_MPU6050 &mpu) {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Checking if the sensor reads 000, 000, 000 and resetting it
  if (a.acceleration.x == 0.00 && a.acceleration.y == 0.00 && a.acceleration.z == 0.00) {
    reset_mpu(mpu);
    mpu.getEvent(&a, &g, &temp);
  }

  // Calculate the change in acceleration
  float delta_x = abs(a.acceleration.x - prev_accel_x);
  float delta_y = abs(a.acceleration.y - prev_accel_y);
  float delta_z = abs(a.acceleration.z - prev_accel_z);

  // Update previous acceleration values
  prev_accel_x = a.acceleration.x;
  prev_accel_y = a.acceleration.y;
  prev_accel_z = a.acceleration.z;

  // Checking if the sensor is in use
  Serial.println("X gyro:" + String(abs(g.gyro.x)));
  Serial.println("Y gyro:" + String(abs(g.gyro.y)));
  Serial.println("Z gyro:" + String(abs(g.gyro.z)));
  Serial.print("X accel: ");
  Serial.println(abs(a.acceleration.x));
  Serial.print("X threshold: ");
  Serial.println(MPU_ACCEL_X_THRESH);
  Serial.print("Y accel: ");
  Serial.println(abs(a.acceleration.y));
  Serial.print("Y threshold: ");
  Serial.println(MPU_ACCEL_Y_THRESH);
  Serial.print("Z accel: ");
  Serial.println(abs(a.acceleration.z));
  Serial.print("Z threshold: ");
  Serial.println(MPU_ACCEL_Z_THRESH);
  Serial.print("Delta X: ");
  Serial.println(delta_x);
  Serial.print("Delta Y: ");
  Serial.println(delta_y);
  Serial.print("Delta Z: ");
  Serial.println(delta_z);
  Serial.print("Change threshold: ");
  Serial.println(ACCEL_CHANGE_THRESH);

  if (delta_x >= ACCEL_CHANGE_THRESH || delta_y >= ACCEL_CHANGE_THRESH || delta_z >= ACCEL_CHANGE_THRESH) {
    Serial.println("Motion detected");
    digitalWrite(led_1, HIGH);
    return true;
  } else {
    Serial.println("Motion not detected");
    digitalWrite(led_1, LOW);
    return false;
  }
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