// SpaceOna Washr ESP32 code V1

// Imports
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ezTime.h>
#include "MPUFunctions.h"
#include "TickFunction.h"
#include "WifiFunctions.h"

#define FIRMWARE_VERSION "0.2"

const char *server_name = "https://api.spaceona.com/update/lafayette.edu/watsonhall/washer/0/false?token=NpLvwbWzkgrpq2UZem9TbfN4s6gcBTiNuaoqA3Ap9S9csrEp";

// Setting up the MPU
Adafruit_MPU6050 mpu;

// Setting up the LEDs
int led_1 = D3;
int led_2 = D2;

// Setting up the wifi client for making HTTPS requests
WiFiClientSecure client;
HTTPClient https;

Timezone myTimezone;
String zone_name = "America/New_York";

void setup(){

  // Setting up the Serial
  Serial.begin(115200);
  delay(1000);

  // Setting up the Serial1 for debugging
  //Serial1.begin(115200, SERIAL_8N1, D7, D6);

  // Setting up the LED
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);

  //Initializing MPU
  mpu_init(mpu, MPU6050_RANGE_8_G, MPU6050_RANGE_500_DEG, MPU6050_BAND_5_HZ);

  //Initializing the Wifi
  wifi_init(server_name, client, https);

  //Setting up the clock
  setClock(myTimezone, zone_name);
}

// Setting up the timers for the tick function state machine
int tick_timer1 = 0;
int tick_timer2 = millis();
int tick_period = 15000; //In milliseconds

// Setting up the timers for the mpu debounce state machine
int mpu_timer1 = 0;
int mpu_timer2 = millis();
int mpu_period = 100; //In milliseconds

bool in_use;

void loop() {

  // Used for ezTime events
  events();

  //Updating the tick functions timers
  tick_timer2 = millis();
  mpu_timer2 = millis();
  
  // Updating the MPUs debouncing state machine
  if((mpu_timer2 - mpu_timer1) >= mpu_period){
    in_use = mpu_tick(mpu);
    //Serial.println("Machine status: ");
    //Serial.println(in_use);
    mpu_timer1 = mpu_timer2;
  }

  //If the server takes a long time to respond, it won't call the mpu function so it doesn't update the status correctly. The ESP32C3 doesn't have multithreading, so need to fix that
  if ((tick_timer2 - tick_timer1) >= tick_period){
    //Serial.println("Tick function reached");
    tickFunction(mpu, server_name, client, https, led_1);
    tick_timer1 = tick_timer2;
  }

}


