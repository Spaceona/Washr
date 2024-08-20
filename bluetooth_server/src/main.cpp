#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "26efde6e-344c-47a4-bf50-78d548220c87"
#define CHARACTERISTIC_UUID "0f066506-bbe6-43cf-8697-4b5ba6b1cc6d"
#define SSID_UUID "ab85696b-0f23-4274-8f5f-886640962c0a"
#define PASSWORD_UUID "05a4ce96-6214-4216-9986-ba51a47c0853"
#define CLIENTKEY_UUID "592e1d83-70a4-4c3e-8917-dcd2c63985fb"
//#define AUTH_UUID "4b1dcf29-6c1d-498b-844b-4169bace5de0"
#define SECRET_UUID "27ea72f0-6f5a-4274-9220-80dcb1778570"



String ssid = "ESPDevNetwork";
String password = "password";
String key = "spaceona";


class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        String value = String(pCharacteristic->getValue().c_str());

        if (value.length() > 0) {
            Serial.println("*********");
            Serial.print("New value: ");
            for (int i = 0; i < value.length(); i++)
                Serial.print(value[i]);

            Serial.println();
            Serial.println("*********");
        }
    }
};

class WiFiCallbacks: public BLECharacteristicCallbacks {
private:
    String uuid;

public:
    WiFiCallbacks(String uuid) : uuid(uuid) {}

    void onWrite(BLECharacteristic *pCharacteristic) override {
        String value = String(pCharacteristic->getValue().c_str());

        Serial.print("onWrite called for UUID: ");
        Serial.println(uuid);

        if (value.length() > 0) {
            Serial.print("New value for ");
            Serial.print(uuid);
            Serial.print(": ");
            Serial.println(value);

            if (uuid == SSID_UUID) {

            } else if (uuid == PASSWORD_UUID) {

            } else if (uuid == CLIENTKEY_UUID) {

            } else if (uuid == SECRET_UUID) {
                Serial.println("Secret: " + value);
            }
        }
    }
};

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
        Serial.println("Client connected");
    };

    void onDisconnect(BLEServer *pServer) {
        Serial.println("Client disconnected - restarting advertising");
        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->start();
    }
};

void setup() {
    Serial.begin(115200);

    BLEDevice::init("WashrProg");
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);

    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE
    );

    pCharacteristic->setCallbacks(new MyCallbacks());

    pCharacteristic->setValue("Hello World");

    BLEDescriptor *pDescriptor = new BLEDescriptor(BLEUUID((uint16_t)0x2901));
    pDescriptor->setValue("A characteristic to read and write");
    pCharacteristic->addDescriptor(pDescriptor);


    //Setting up the characteristic for the SSID
    BLECharacteristic *ssidCharacteristic = pService->createCharacteristic(
            SSID_UUID,
            BLECharacteristic::PROPERTY_READ
    );
    ssidCharacteristic->setCallbacks(new WiFiCallbacks(SSID_UUID));
    ssidCharacteristic->setValue(ssid.c_str());

    //Descriptor for the SSID characteristic (so it says "SSID" in an app)
    BLEDescriptor *ssidDescriptor = new BLEDescriptor(BLEUUID((uint16_t)0x2901));
    ssidDescriptor->setValue("SSID");
    ssidCharacteristic->addDescriptor(ssidDescriptor);
    ssidCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);

    //Setting up the characteristic for the password
    BLECharacteristic *passwordCharacteristic = pService->createCharacteristic(
            PASSWORD_UUID,
            BLECharacteristic::PROPERTY_READ
    );
    passwordCharacteristic->setCallbacks(new WiFiCallbacks(PASSWORD_UUID));
    passwordCharacteristic->setValue(password.c_str());

    //Descriptor for the password characteristic (so it says "Password" in an app)
    BLEDescriptor *passwordDescriptor = new BLEDescriptor(BLEUUID((uint16_t)0x2901));
    passwordDescriptor->setValue("Password");
    passwordCharacteristic->addDescriptor(passwordDescriptor);
    passwordCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);

    //Setting up the characteristic for the client key
    BLECharacteristic *keyCharacteristic = pService->createCharacteristic(
            CLIENTKEY_UUID,
            BLECharacteristic::PROPERTY_READ
    );
    keyCharacteristic->setCallbacks(new WiFiCallbacks(CLIENTKEY_UUID));
    keyCharacteristic->setValue(key.c_str());

    //Descriptor for the key characteristic (so it says "Client Key" in an app)
    BLEDescriptor *keyDescriptor = new BLEDescriptor(BLEUUID((uint16_t)0x2901));
    keyDescriptor->setValue("Client Key");
    keyCharacteristic->addDescriptor(keyDescriptor);
    keyCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);

    //Starting the server and advertising it
    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->start();

    //Setting up the security
    BLESecurity *pSecurity = new BLESecurity();
    pSecurity->setStaticPIN(123456);


}

void loop() {
    // put your main code here, to run repeatedly:
    delay(2000);
}