
#include "protocol/KellyFrameCodec.h"

bool KellyFrameCodec::isSupportedCommand(uint8_t command)
{
    return command == A2D_BATCH ||
           command == MONITOR1 ||
           command == MONITOR2;
}

uint8_t KellyFrameCodec::checksum(
    const uint8_t* bytes,
    size_t length
)
{
    if (bytes == nullptr)
    {
        return 0;
    }

    uint8_t sum = 0;

    for (size_t i = 0; i < length; ++i)
    {
        sum = static_cast<uint8_t>(sum + bytes[i]);
    }

    return sum;
}

bool KellyFrameCodec::buildRequest(
    uint8_t command,
    uint8_t out[REQUEST_SIZE]
)
{
    if (out == nullptr || !isSupportedCommand(command))
    {
        return false;
    }

    out[0] = command;
    out[1] = 0x00;
    out[2] = checksum(out, 2);

    return true;
}

bool KellyFrameCodec::validateResponse(
    const uint8_t* packet,
    size_t length,
    uint8_t expectedCommand
)
{
    if (packet == nullptr)
    {
        return false;
    }

    if (!isSupportedCommand(expectedCommand))
    {
        return false;
    }

    if (length != RESPONSE_SIZE)
    {
        return false;
    }

    // Controller must echo the requested command
    if (packet[0] != expectedCommand)
    {
        return false;
    }

    // Payload length must be 16 bytes
    if (packet[1] != DATA_SIZE)
    {
        return false;
    }

    const uint8_t calculated =
        checksum(packet, RESPONSE_SIZE - 1);

    return calculated == packet[RESPONSE_SIZE - 1];
}

bool KellyFrameCodec::extractPayload(
    const uint8_t* packet,
    size_t length,
    uint8_t expectedCommand,
    uint8_t out[DATA_SIZE]
)
{
    if (out == nullptr)
    {
        return false;
    }

    if (!validateResponse(packet, length, expectedCommand))
    {
        return false;
    }

    for (size_t i = 0; i < DATA_SIZE; ++i)
    {
        out[i] = packet[i + 2];
    }

    return true;
}

bool KellyFrameCodec::buildTestResponse(
    uint8_t command,
    const uint8_t data[DATA_SIZE],
    uint8_t out[RESPONSE_SIZE]
)
{
    if (data == nullptr || out == nullptr)
    {
        return false;
    }

    if (!isSupportedCommand(command))
    {
        return false;
    }

    out[0] = command;
    out[1] = static_cast<uint8_t>(DATA_SIZE);

    for (size_t i = 0; i < DATA_SIZE; ++i)
    {
        out[i + 2] = data[i];
    }

    out[RESPONSE_SIZE - 1] =
        checksum(out, RESPONSE_SIZE - 1);

    return true;
}
