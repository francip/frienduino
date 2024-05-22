#ifndef MIC_HANDLER_H
#define MIC_HANDLER_H

#include "CodecType.h"

#define CODEC_BUFFER_SIZE 16000

class MicHandler
{
public:
    static void onNewData();

    MicHandler(const char channels = 1, const int frequency = 16000);

    void initialize();
    void begin();
    bool isActive();
    void end();

    CodecType getCodecType();
    short getSamplesAvailable();
    int getSamplesData(short *data, int length);

private :
    static MicHandler *instance;

    bool active;

    const char channels;
    const int frequency;

    bool audioStreaming;
    short sampleBuffer[CODEC_BUFFER_SIZE];
    volatile int writePos;
    volatile int readPos;

    int writeToBuffer(short *data, int length);

    void newData();
};

#endif