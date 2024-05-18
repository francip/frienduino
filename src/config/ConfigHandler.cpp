#include "ConfigHandler.h"

ConfigHandler::ConfigHandler(StorageHandler &storage) : storage(storage) {}

void ConfigHandler::initialize()
{
    loadConfig();
}

const char *ConfigHandler::getName()
{
    return name;
}

void ConfigHandler::updateName(const String &newName)
{
    newName.toCharArray(name, sizeof(name));
    saveConfig();
}

void ConfigHandler::loadConfig()
{
    if (!storage.readValue("name", name, sizeof(name)))
    {
        strcpy(name, defaultName);
    }
}

void ConfigHandler::saveConfig()
{
    storage.writeValue("name", name);
}

void ConfigHandler::reset()
{
    storage.removeValue("name");
}
