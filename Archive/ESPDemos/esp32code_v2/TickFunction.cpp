#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "TickFunction.h"
#include "MPUFunctions.h"



// Setting the states for the tick function
enum States {
  Start,
  Transmit
} Sensor_State;

// Setting up the tick function
void tickFunction(Adafruit_MPU6050 &mpu, const char* server_name, WiFiClientSecure &client, HTTPClient &https, int &led_1) {

  // Transitions
  switch (Sensor_State)
  {
  case Start:
    Sensor_State = Transmit;
    break;
  case Transmit:
    Sensor_State = Transmit;
    break;
  default:
    Sensor_State = Start;
    break;
  }

  // State logic
  switch (Sensor_State) {
  case Start:
    // State logic would go here if there was any
    break;
  case Transmit:

    digitalWrite(led_1, LOW);

    //Serial.println("Entered Transmit stage!");
    // Checking to make sure the wifi is configured
    if (WiFi.status() != WL_CONNECTED) {
      while (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nWifi is offline, trying to reconnect...");
        WiFi.reconnect();
        delay(500);
      }
      Serial.println("\nWifi has reconnected");
      Serial.print("Local ESP32 IP: ");
      Serial.println(WiFi.localIP());
    } else {
      
      Serial.print("Local ESP32 IP: ");
      Serial.println(WiFi.localIP());

      // Transmitting the data
      Serial.println("Starting HTTPS connection...");
      if (!https.begin(client, server_name)) {
        Serial.println("Failed to start HTTPS connection");
        return;
      }
      https.addHeader("Content-Type", "application/json");
      String httpsPostData = get_mpu_data(mpu);
      Serial.println("Sending POST request...");
      int httpsResponseCode = https.POST(httpsPostData);
      Serial.print("HTTPS Response code: ");
      Serial.println(httpsResponseCode);
      //Should probably add better error code handling here
      if (httpsResponseCode <= 0) {
        Serial.println("Failed to send POST request");
        return;
      } else {
        digitalWrite(led_1, HIGH);
        delay(100);
      }
      Serial.println("");
      digitalWrite(led_1, LOW);
      
    }
    break;
  default:
    break;
  }
}