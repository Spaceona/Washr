#include <Arduino.h>

#define LED1_PIN 6
#define LED2_PIN 10

void setup() {
// write your initialization code here
    //Serial.begin(115200);
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
}

void loop() {
// write your code here
    //Serial.println("Hello World");
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    delay(1000);
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    delay(1000); // Wait for 1 second
}