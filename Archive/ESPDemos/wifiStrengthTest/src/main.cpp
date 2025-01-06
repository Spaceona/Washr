#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "ESPDevNetwork";
const char* password = "password";

void setup(){
    Serial.begin(115200);
    delay(2000);

    Serial.println("MAC Address: " + WiFi.macAddress());

    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("Signal strength (RSSI): " + String(WiFi.RSSI()));
}

void loop(){

    if(WiFi.status() != WL_CONNECTED){
        while(WiFi.status() != WL_CONNECTED){
            Serial.println("\nWifi is offline, trying to reconnect...");
            WiFi.reconnect();
            delay(500);
        }
        Serial.println("\nWifi has reconnected");
        Serial.print("Local ESP32 IP: ");
        Serial.println(WiFi.localIP());

    }


}