#ifndef FLASHSTORAGEHANDLER_H
#define FLASHSTORAGEHANDLER_H

#include "StorageHandler.h"

class FlashStorageHandler : public StorageHandler
{
public:
    FlashStorageHandler();
    bool writeValue(const char *key, const char *value) override;
    bool readValue(const char *key, char *buffer, size_t bufferSize) override;
    bool removeValue(const char *key) override;
};

#endif // FLASHSTORAGEHANDLER_H