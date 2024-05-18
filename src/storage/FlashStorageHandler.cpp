#include <Arduino.h>

#include "FlashStorageHandler.h"

#include <string.h>
#include <mbed_error.h>
#include <kvstore_global_api.h>

FlashStorageHandler::FlashStorageHandler() {}

bool FlashStorageHandler::writeValue(const char *key, const char *value)
{
    int ret = kv_set(key, value, strlen(value), 0);
    return ret == MBED_SUCCESS;
}

bool FlashStorageHandler::readValue(const char *key, char *buffer, size_t bufferSize)
{
    kv_info_t infoBuffer;
    int ret = kv_get_info(key, &infoBuffer);

    if (ret != MBED_SUCCESS)
    {
        return false;
    }

    if (infoBuffer.size > bufferSize)
    {
        return false;
    }

    ret = kv_get(key, buffer, infoBuffer.size, nullptr);

    return ret == MBED_SUCCESS;
}

bool FlashStorageHandler::removeValue(const char *key)
{
    int ret = kv_remove(key);
    return ret == MBED_SUCCESS;
}