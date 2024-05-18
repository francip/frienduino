#ifndef STATUS_MANAGER_H
#define STATUS_MANAGER_H

#include "../config/ConfigHandler.h"
#include "../ble/BLEHandler.h"
#include "../audio/MicHandler.h"

enum DeviceStatus
{
    BOOTING,
    IDLE,
    CONNECTED,
    STREAMING,
    ERROR
};

class StatusManager
{
public:
    StatusManager(
        ConfigHandler &configHandler,
        BLEHandler &bleHandler,
        MicHandler &micHandler);
    void initialize();
    void setStatus(DeviceStatus newStatus);
    DeviceStatus getStatus();
private:
    static StatusManager* instance;

    ConfigHandler &configHandler;
    BLEHandler &bleHandler;
    MicHandler &micHandler;

    DeviceStatus status;
};

#endif