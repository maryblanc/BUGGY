
#pragma once

#include <stddef.h>
#include <stdint.h>

class KellyFrameCodec
{
public:
    static constexpr size_t REQUEST_SIZE = 3;
    static constexpr size_t DATA_SIZE = 16;
    static constexpr size_t RESPONSE_SIZE = 19;

    static constexpr uint8_t A2D_BATCH = 0x1B;
    static constexpr uint8_t MONITOR1 = 0x3A;
    static constexpr uint8_t MONITOR2 = 0x3B;

    // Supported read-only commands
    static bool isSupportedCommand(uint8_t command);

    // Build a three-byte read request
    static bool buildRequest(
        uint8_t command,
        uint8_t out[REQUEST_SIZE]
    );

    // Checksum = sum of all bytes modulo 256
    static uint8_t checksum(
        const uint8_t* bytes,
        size_t length
    );

    // Validate a complete controller response
    static bool validateResponse(
        const uint8_t* packet,
        size_t length,
        uint8_t expectedCommand
    );

    // Extract the 16 payload bytes
    static bool extractPayload(
        const uint8_t* packet,
        size_t length,
        uint8_t expectedCommand,
        uint8_t out[DATA_SIZE]
    );

    // Build a simulated response for testing
    static bool buildTestResponse(
        uint8_t command,
        const uint8_t data[DATA_SIZE],
        uint8_t out[RESPONSE_SIZE]
    );
};
