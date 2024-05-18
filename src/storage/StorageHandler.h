#ifndef STORAGEHANDLER_H
#define STORAGEHANDLER_H

#include <cstddef>

class StorageHandler
{
public:
    virtual ~StorageHandler() {}
    virtual bool writeValue(const char *key, const char *value) = 0;
    virtual bool readValue(const char *key, char *buffer, size_t bufferSize) = 0;
    virtual bool removeValue(const char *key) = 0;
};

#endif // STORAGEHANDLER_H