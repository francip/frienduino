#ifndef CONFIG_HANDLER_H
#define CONFIG_HANDLER_H

#include <Arduino.h>
#include <WString.h>

#include "../storage/StorageHandler.h"

typedef void (*WifiCredentialsUpdateHandler)();

class ConfigHandler
{
public:
    ConfigHandler(StorageHandler &storage);
    void initialize();

    const char *getName();
    void updateName(const String &newName);

    void reset();

private:
    StorageHandler &storage;

    const char *defaultName = "Friend";

    char name[100];

    void loadConfig();
    void saveConfig();

    WifiCredentialsUpdateHandler wifiCredentialsUpdateCallback;
};

#endif