#include <Arduino.h>
#include <ArduinoBLE.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "26efde6e-344c-47a4-bf50-78d548220c87"
#define CHARACTERISTIC_UUID "0f066506-bbe6-43cf-8697-4b5ba6b1cc6d"
#define SSID_UUID "ab85696b-0f23-4274-8f5f-886640962c0a"
#define PASSWORD_UUID "05a4ce96-6214-4216-9986-ba51a47c0853"
#define CLIENTKEY_UUID "592e1d83-70a4-4c3e-8917-dcd2c63985fb"
//#define AUTH_UUID "4b1dcf29-6c1d-498b-844b-4169bace5de0"
#define CLIENTNAME_UUID "27ea72f0-6f5a-4274-9220-80dcb1778570"



String ssid = "ESPDevNetwork";
String password = "password";
String key = "testkey";
String clientName = "test2";


void setup(){
    Serial.begin(9600);
    while (!Serial);

    // set advertised local name and service UUID:
    BLE.setLocalName("WashrProg");
    BLE.setAdvertisedService(SERVICE_UUID);

    // add service
    BLE.addService(SERVICE_UUID);

    // add characteristic
    BLECharacteristic ssidCharacteristic = BLECharacteristic(SSID_UUID, BLERead);
    BLECharacteristic passwordCharacteristic = BLECharacteristic(PASSWORD_UUID, BLERead);
    BLECharacteristic clientKeyCharacteristic = BLECharacteristic(CLIENTKEY_UUID, BLERead);
    BLECharacteristic secretCharacteristic = BLECharacteristic(CLIENTNAME_UUID, BLERead);

    // set values for characteristics
    ssidCharacteristic.writeValue(ssid.c_str());
    passwordCharacteristic.setValue(password);
    clientKeyCharacteristic.setValue(key);
    secretCharacteristic.setValue(clientName);

    // add characteristics to the service
    BLE.addCharacteristic(ssidCharacteristic);
    BLE.addCharacteristic(passwordCharacteristic);
    BLE.addCharacteristic(clientKeyCharacteristic);
    BLE.addCharacteristic(secretCharacteristic);

    // start advertising
    BLE.advertise();
    Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
    // put your main code here, to run repeatedly:
    delay(2000);
}