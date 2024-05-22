#ifndef BLE_HANDLER_H
#define BLE_HANDLER_H

#include <ArduinoBLE.h>

#include "../config/ConfigHandler.h"

#define NET_BUFFER_HEADER_SIZE 3
#define NET_BUFFER_DATA_SIZE 160

class BLEHandler
{
public:
    static void onNameWritten(BLEDevice central, BLECharacteristic characteristic);
    static void onResetWritten(BLEDevice central, BLECharacteristic characteristic);

    BLEHandler(ConfigHandler &configHandler);

    void begin();
    bool isActive();
    void end();
    void startAdvertising();
    bool isAdvertising();
    void stopAdvertising();
    void poll();

    void writeAudio(short *data, int length);
    bool isClientConnected();
    bool isClientListening();

private :
    static BLEHandler *instance;

    bool active;
    bool advertising;
    short nextPacketIndex;

    ConfigHandler &configHandler;

    BLEService deviceControlService;
    BLEStringCharacteristic nameCharacteristic;
    BLEBoolCharacteristic resetCharacteristic;
    BLECharacteristic audioCharacteristic;
    BLEByteCharacteristic codecCharacteristic;

    void nameWritten(BLEDevice central, BLECharacteristic characteristic);
    void resetWritten(BLEDevice central, BLECharacteristic characteristic);
};

#endif