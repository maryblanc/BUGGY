#pragma once

#include <Arduino.h>
#include <stdint.h>

class KellyProtocol
{
public:
    bool begin();

    bool readMonitor1(uint8_t* data);
    bool readMonitor2(uint8_t* data);
    bool readMonitor3(uint8_t* data);
    bool testPacket(const uint8_t* packet, uint8_t* data);

private:
    static constexpr size_t PACKET_SIZE = 19;
    static constexpr size_t DATA_SIZE = 16;
    static constexpr uint8_t MONITOR1_CMD = 0x3A;
    static constexpr uint8_t MONITOR2_CMD = 0x3B;
    static constexpr uint8_t MONITOR3_CMD = 0x3C;

    static constexpr unsigned long RESPONSE_TIMEOUT_MS = 300;
    static constexpr uint8_t MAX_RETRIES = 1;

    bool readMonitor(uint8_t command, uint8_t* data);

    void drainReceiveBuffer();

    bool sendCommand(uint8_t command);

    bool receivePacket(
        uint8_t expectedCommand,
        uint8_t* data
    );

    uint8_t calculateChecksum(
        const uint8_t* packet
    ) const;

    bool validatePacket(
        const uint8_t* packet,
        uint8_t expectedCommand
    ) const;
};