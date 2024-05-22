#include "BLEHandler.h"
#include "../mic/CodecType.h"

#include <WiFiNINA.h>

#define BLE_NAME_LENGTH 100
#define BLE_AUDIO_LENGTH (NET_BUFFER_HEADER_SIZE + NET_BUFFER_DATA_SIZE)

BLEHandler *BLEHandler::instance = nullptr;

BLEHandler::BLEHandler(ConfigHandler &configHandler)
    : configHandler(configHandler),
      deviceControlService("19B10000-E8F2-537E-4F6C-D104768A1214"),
      audioCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, BLE_AUDIO_LENGTH),
      codecCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify),
      nameCharacteristic("19B10010-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite, BLE_NAME_LENGTH),
      resetCharacteristic("19B10020-E8F2-537E-4F6C-D104768A1214", BLEWrite),
      active(false),
      advertising(false),
      nextPacketIndex(0)
{
    BLEHandler::instance = this;
}

void BLEHandler::begin()
{
    if (BLE.begin())
    {
        active = true;

        BLE.setLocalName(configHandler.getName());
        BLE.setDeviceName(configHandler.getName());

        BLE.setAdvertisedService(deviceControlService);

        deviceControlService.addCharacteristic(audioCharacteristic);
        deviceControlService.addCharacteristic(codecCharacteristic);
        deviceControlService.addCharacteristic(nameCharacteristic);
        deviceControlService.addCharacteristic(resetCharacteristic);

        nameCharacteristic.setEventHandler(BLEWritten, BLEHandler::onNameWritten);
        resetCharacteristic.setEventHandler(BLEWritten, BLEHandler::onResetWritten);

        BLE.addService(deviceControlService);

        nameCharacteristic.writeValue(configHandler.getName());
        // TODO: Get this from ConfigHandler
        codecCharacteristic.writeValue((uint8_t)CodecType::PCM_8);
    }
}

bool BLEHandler::isActive()
{
    return active;
}

void BLEHandler::end()
{
    if (!active)
    {
        return;
    }

    if (advertising)
    {
        BLE.stopAdvertise();
    }

    BLE.end();

    active = false;
}

void BLEHandler::startAdvertising()
{
    if (!active)
    {
        return;
    }

    BLE.advertise();
    advertising = true;
}

void BLEHandler::stopAdvertising()
{
    if (!active)
    {
        return;
    }

    BLE.stopAdvertise();
    advertising = false;
}

bool BLEHandler::isAdvertising()
{
    return advertising;
}

void BLEHandler::poll()
{
    if (!active)
    {
        return;
    }

    BLE.poll();
}

void BLEHandler::nameWritten(BLEDevice central, BLECharacteristic characteristic)
{
    String newName = nameCharacteristic.value();
    configHandler.updateName(newName);

    BLE.stopAdvertise();
    BLE.setLocalName(configHandler.getName());
    BLE.setDeviceName(configHandler.getName());
    BLE.advertise();

    nameCharacteristic.writeValue(configHandler.getName());
}

void BLEHandler::resetWritten(BLEDevice central, BLECharacteristic characteristic)
{
    configHandler.reset();

    nameCharacteristic.writeValue(configHandler.getName());
}

void BLEHandler::onNameWritten(BLEDevice central, BLECharacteristic characteristic)
{
    if (BLEHandler::instance)
    {
        BLEHandler::instance->nameWritten(central, characteristic);
    }
}

void BLEHandler::onResetWritten(BLEDevice central, BLECharacteristic characteristic)
{
    if (BLEHandler::instance)
    {
        BLEHandler::instance->resetWritten(central, characteristic);
    }
}

void BLEHandler::writeAudio(short *data, int length)
{
    if (!active)
    {
        return;
    }

    int packetSize = (NET_BUFFER_DATA_SIZE) / sizeof(short);
    int packets = length / packetSize;

    for (int i = 0; i < packets; i++)
    {
        uint8_t packet[BLE_AUDIO_LENGTH];

        short id = nextPacketIndex++;
        packet[0] = id & 0xFF;
        packet[1] = (id >> 8) & 0xFF;
        packet[2] = i;

        memcpy(packet + NET_BUFFER_HEADER_SIZE, data + i * packetSize, NET_BUFFER_DATA_SIZE);

        audioCharacteristic.writeValue(packet, BLE_AUDIO_LENGTH);

        // delay(5);
    }
}

bool BLEHandler::isClientConnected()
{
    if (!active)
    {
        return false;
    }

    return BLE.connected();
}

bool BLEHandler::isClientListening()
{
    if (!active)
    {
        return false;
    }

    return audioCharacteristic.subscribed();
}