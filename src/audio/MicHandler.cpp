#include "MicHandler.h"

#include <PDM.h>

MicHandler *MicHandler::instance = nullptr;

#define MIC_GAIN 64
#define MIC_BUFFER_SAMPLES 160
#define MIC_BUFFER_SIZE (MIC_BUFFER_SAMPLES * sizeof(short))

volatile bool pdmHasData = false;

MicHandler::MicHandler(const char channels, const int frequency)
    : active(false),
      channels(channels),
      frequency(frequency),
      audioStreaming(false),
      readPos(0),
      writePos(0)
{
    MicHandler::instance = this;
}

void MicHandler::initialize()
{
    // PDM.setGain(MIC_GAIN);
    // PDM.setBufferSize(MIC_BUFFER_SIZE);
    PDM.onReceive(MicHandler::onNewData);
}

void MicHandler::begin()
{
    if (PDM.begin(channels, frequency))
    {
        active = true;
    }
}

bool MicHandler::isActive()
{
    return active;
}

void MicHandler::end()
{
    if (!active)
    {
        return;
    }

    PDM.end();

    readPos = 0;
    writePos = 0;

    active = false;
}

CodecType MicHandler::getCodecType()
{
    return CodecType::PCM_8;
}

short MicHandler::getSamplesAvailable()
{
    int samplesAvailable = readPos <= writePos ? writePos - readPos : CODEC_BUFFER_SIZE - readPos + writePos;
    Serial.println("Samples available: " + String(samplesAvailable));
    return samplesAvailable;
}

int MicHandler::getSamplesData(short *data, int length)
{
    int i = 0;
    while (i < length && readPos != writePos)
    {
        data[i] = sampleBuffer[readPos];

        readPos = (readPos + 1) % CODEC_BUFFER_SIZE;

        i++;
    }

    return i;
}

int MicHandler::writeToBuffer(short *data, int length)
{
    int i = 0;
    while (i < length)
    {
        if ((writePos + 1) % CODEC_BUFFER_SIZE == readPos)
        {
            readPos = (readPos + 1) % CODEC_BUFFER_SIZE;
        }

        sampleBuffer[writePos] = data[i];

        writePos = (writePos + 1) % CODEC_BUFFER_SIZE;

        i++;
    }

    return i;
}

void MicHandler::newData()
{
    int bytesAvailable = PDM.available();
    short data[MIC_BUFFER_SAMPLES];
    int bytesRead = PDM.read(data, bytesAvailable);
    writeToBuffer((short *)data, bytesRead / sizeof(short));
}

void MicHandler::onNewData()
{
    if (MicHandler::instance)
    {
        MicHandler::instance->newData();
    }
}
