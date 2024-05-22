#include "src/storage/FlashStorageHandler.h"
#include "src/config/ConfigHandler.h"
#include "src/ble/BLEHandler.h"
#include "src/mic/MicHandler.h"
#include "src/status/StatusManager.h"

FlashStorageHandler storageHandler;
ConfigHandler configHandler((StorageHandler &)storageHandler);
BLEHandler bleHandler(configHandler);
MicHandler micHandler;
StatusManager statusManager(configHandler, bleHandler, micHandler);

#include <Arduino.h>

void setup()
{
    statusManager.initialize();
}

void loop()
{
    DeviceStatus status = statusManager.getStatus();
}
