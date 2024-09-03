#include <Arduino.h>
//#include <BLEDevice.h>
#include <ArduinoBLE.h>

//BLE Server name (the other ESP32 name running the server sketch)
#define bleServerName "WashrProg"

#define SERVICE_UUID        "26efde6e-344c-47a4-bf50-78d548220c87"
#define CHARACTERISTIC_UUID "0f066506-bbe6-43cf-8697-4b5ba6b1cc6d"
#define SSID_UUID "ab85696b-0f23-4274-8f5f-886640962c0a"
#define PASSWORD_UUID "05a4ce96-6214-4216-9986-ba51a47c0853"
#define CLIENTKEY_UUID "592e1d83-70a4-4c3e-8917-dcd2c63985fb"
#define AUTH_UUID "4b1dcf29-6c1d-498b-844b-4169bace5de0"  //currently unused
#define SECRET_UUID "27ea72f0-6f5a-4274-9220-80dcb1778570" //currently unused

static BLEUuid serviceUUID(SERVICE_UUID);
static BLEUuid charUUID(CHARACTERISTIC_UUID);
static BLEUuid ssidUUID(SSID_UUID);
static BLEUuid passwordUUID(PASSWORD_UUID);
static BLEUuid clientKeyUUID(CLIENTKEY_UUID);
static BLEUuid authUUID(AUTH_UUID);
static BLEUuid secretUUID(SECRET_UUID);

//Flags for if it should or is connected
static boolean doConnect = false;
static boolean connected = false;

BLEDevice peripheral;
BLEService targetService;

//Address of the peripheral device. Address will be found during scanning...
//static BLEAddress *pServerAddress;

//static BLERemoteCharacteristic* ssidCharacteristic;
//static BLERemoteCharacteristic* passwordCharacteristic;
//static BLERemoteCharacteristic* clientKeyCharacteristic;

void setup() {
// write your initialization code here
  Serial.begin(115200);

  while(!Serial);
  if (!BLE.begin()){
    Serial.println("BLE initialization failed...");
  }
  BLE.scan();

  while(!(peripheral.connected())){
    peripheral = BLE.available(); 
    Serial.println("Peripheral: " + peripheral.localName());
    if (peripheral.localName().equals("WashrProg")){
      BLE.stopScan();
      peripheral.connect();
      Serial.println("Connected to service...");
    }

  }
}

void loop() {
// write your c

}