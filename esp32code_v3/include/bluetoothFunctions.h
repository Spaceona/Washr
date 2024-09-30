// Created by Robbie Leslie
// Modified by

#ifndef ESP32CODE_V3_BLUETOOTHFUNCTIONS_H
#define ESP32CODE_V3_BLUETOOTHFUNCTIONS_H

#include <ArduinoBLE.h>

void bluetoothInit();
void bluetoothShutdown();
void onCentralDisconnected(BLEDevice central);
void onSSIDWritten(BLEDevice central, BLECharacteristic characteristic);
void onPasswordWritten(BLEDevice central, BLECharacteristic characteristic);
void onClientKeyWritten(BLEDevice central, BLECharacteristic characteristic);
void onClientNameWritten(BLEDevice central, BLECharacteristic characteristic);

#endif //ESP32CODE_V3_BLUETOOTHFUNCTIONS_H
