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
#define SECRET_UUID "27ea72f0-6f5a-4274-9220-80dcb1778570"

#define SERVICE_UUID "26efde6e-344c-47a4-bf50-78d548220c87"
#define CHARACTERISTIC_UUID "0f066506-bbe6-43cf-8697-4b5ba6b1cc6d"
#define SSID_UUID "ab85696b-0f23-4274-8f5f-886640962c0a"
#define PASSWORD_UUID "05a4ce96-6214-4216-9986-ba51a47c0853"
#define CLIENTKEY_UUID "592e1d83-70a4-4c3e-8917-dcd2c63985fb"
#define AUTH_UUID "4b1dcf29-6c1d-498b-844b-4169bace5de0"    //currently unused
#define SECRET_UUID "27ea72f0-6f5a-4274-9220-80dcb1778570"  //currently unused

String ssid = "ESPDevNetwork";
String password = "password";
String key = "spaceona";


void setup(){
    Serial.begin(115200);

    while (!Serial){
        delay(10);
    }

    if (!BLE.begin()) {
        Serial.println("BLE initialization failed...");
    }
    BLE.scanForUuid("SERVICE_UUID");
}

void loop() {
    // check if a peripheral has been discovered
    BLEDevice peripheral = BLE.available();

    if (peripheral) {
        // discovered a peripheral, print out address, local name, and advertised service
        Serial.print("Found ");
        Serial.print(peripheral.address());
        Serial.print(" '");
        Serial.print(peripheral.localName());
        Serial.print("' ");
        Serial.print(peripheral.advertisedServiceUuid());
        Serial.println();
    }

}