#include "StatusManager.h"

#include <Arduino.h>

StatusManager *StatusManager::instance = nullptr;

StatusManager::StatusManager(
    ConfigHandler &configHandler,
    BLEHandler &bleHandler,
    MicHandler &micHandler)
    : status(DeviceStatus::BOOTING),
      configHandler(configHandler),
      bleHandler(bleHandler),
      micHandler(micHandler)
{
    StatusManager::instance = this;
}

void StatusManager::initialize()
{
    Serial.begin(9600);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Starting...");

    configHandler.initialize();

    micHandler.initialize();

    bleHandler.begin();
    bleHandler.startAdvertising();

    setStatus(DeviceStatus::IDLE);
}

void StatusManager::setStatus(DeviceStatus newStatus)
{
    if (status == newStatus)
    {
        return;
    }

    status = newStatus;

    Serial.print("New status: ");
    Serial.println(status);

    switch (status)
    {
        case DeviceStatus::IDLE:
        {
            digitalWrite(LED_BUILTIN, LOW);
            Serial.println("Waiting...");

            micHandler.end();

            break;
        }

        case DeviceStatus::CONNECTED:
        {
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.println("Connected");

            micHandler.end();

            break;
        }

        case DeviceStatus::STREAMING:
        {
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.println("Streaming...");

            micHandler.begin();

            if (!micHandler.isActive())
            {
                setStatus(DeviceStatus::ERROR);
            }

            break;
        }

        case DeviceStatus::ERROR:
        {
            bleHandler.end();

            Serial.println("Error");

            int ledState = HIGH;
            for (;;)
            {
                digitalWrite(LED_BUILTIN, ledState);
                ledState = (ledState == LOW) ? HIGH : LOW;
                delay(100);
            }
            break;
        }

        default:
            break;
    }
}

DeviceStatus StatusManager::getStatus()
{
    if (!bleHandler.isActive())
    {
        setStatus(DeviceStatus::ERROR);
    }

    bleHandler.poll();

    if (status == DeviceStatus::IDLE)
    {
        if (bleHandler.isClientConnected())
        {
            setStatus(DeviceStatus::CONNECTED);
        }
    }
    else if (status == DeviceStatus::CONNECTED)
    {
        if (!bleHandler.isClientConnected())
        {
            setStatus(DeviceStatus::IDLE);
        }
        else if (bleHandler.isClientListening())
        {
            setStatus(DeviceStatus::STREAMING);
        }
    }
    else if (status == DeviceStatus::STREAMING)
    {
        if (!bleHandler.isClientConnected())
        {
            setStatus(DeviceStatus::IDLE);
        }
        else if (!bleHandler.isClientListening())
        {
            setStatus(DeviceStatus::CONNECTED);
        }
        else
        {
            const int packetSize = (NET_BUFFER_DATA_SIZE) / sizeof(short);

            int samplesAvailable = micHandler.getSamplesAvailable();
            int packetsAvailable = samplesAvailable / packetSize;

            if (packetsAvailable > 0)
            {
                short sampleBuffer[packetsAvailable * packetSize];
                int samplesRead = micHandler.getSamplesData(sampleBuffer, packetsAvailable * packetSize);
                if (samplesRead > 0)
                {
                    bleHandler.writeAudio(sampleBuffer, samplesRead);
                }
            }
        }
    }

    return status;
}
