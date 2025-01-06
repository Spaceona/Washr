#include <Arduino.h>
#include <ICM42670P.h>
#include <Wire.h>
#include "IMUFunctions.h"

#define LED1_PIN 6
#define LED2_PIN 10

void setup() {

    Serial.begin(115200);
    delay(2000);
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);


    int ret;
    Serial.begin(115200);
    while(!Serial) {

    }

    IMUInit();

}

// Setting up the timers for the mpu debounce state machine
unsigned long mpu_timer1 = 0;
unsigned long mpu_timer2 = millis();
int mpu_period = 50;

bool in_use;

void loop() {

    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);

    mpu_timer2 = millis();

    if((mpu_timer2 - mpu_timer1) >= mpu_period) {
        //Serial.println("Hello World");
        digitalWrite(LED1_PIN, HIGH);
        digitalWrite(LED2_PIN, HIGH);

        in_use = IMU_tick();
        Serial.print("Machine status: ");
        Serial.println(in_use);
        mpu_timer1 = mpu_timer2;
    }

}