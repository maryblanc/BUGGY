#include "controllers/KellyProtocol.h"
#include "Config.h"

bool KellyProtocol::begin()
{
    Serial2.begin(UART_BAUDRATE);

    return true;
}


bool KellyProtocol::readMonitor1(uint8_t* data)
{
    return readMonitor(MONITOR1_CMD, data);
}


bool KellyProtocol::readMonitor2(uint8_t* data)
{
    return readMonitor(MONITOR2_CMD, data);
}


bool KellyProtocol::readMonitor3(uint8_t* data)
{
    return readMonitor(MONITOR3_CMD, data);
}


bool KellyProtocol::readMonitor(
    uint8_t command,
    uint8_t* data
)
{
    if (data == nullptr)
    {
        return false;
    }

    for (uint8_t attempt = 0; attempt <= MAX_RETRIES; ++attempt)
    {
        // --------------------------------------------------
        // 1. Remove stale bytes from RX buffer
        // --------------------------------------------------

        drainReceiveBuffer();


        // --------------------------------------------------
        // 2. Send command
        // --------------------------------------------------

        if (!sendCommand(command))
        {
            continue;
        }


        // --------------------------------------------------
        // 3. Receive response
        // --------------------------------------------------

        if (receivePacket(command, data))
        {
            return true;
        }
    }

    return false;
}


void KellyProtocol::drainReceiveBuffer()
{
    while (Serial2.available() > 0)
    {
        Serial2.read();
    }
}


bool KellyProtocol::sendCommand(uint8_t command)
{
    uint8_t packet[3];

    packet[0] = command;
    packet[1] = 0x00;
    packet[2] = command;

    size_t written = Serial2.write(packet, sizeof(packet));

    Serial2.flush();

    return written == sizeof(packet);
}


bool KellyProtocol::receivePacket(
    uint8_t expectedCommand,
    uint8_t* data
)
{
    uint8_t packet[PACKET_SIZE];

    unsigned long startTime = millis();

    size_t received = 0;

    while (received < PACKET_SIZE)
    {
        if (Serial2.available() > 0)
        {
            packet[received] = Serial2.read();
            received++;
        }

        if (millis() - startTime >= RESPONSE_TIMEOUT_MS)
        {
            return false;
        }
    }


    // --------------------------------------------------
    // Validate complete packet
    // --------------------------------------------------

    if (!validatePacket(packet, expectedCommand))
    {
        return false;
    }


    // --------------------------------------------------
    // Extract 16-byte monitor payload
    //
    // packet:
    // [0]     command
    // [1]     length = 0x10
    // [2..17] data
    // [18]    checksum
    // --------------------------------------------------

    for (size_t i = 0; i < DATA_SIZE; ++i)
    {
        data[i] = packet[i + 2];
    }

    return true;
}


uint8_t KellyProtocol::calculateChecksum(
    const uint8_t* packet
) const
{
    uint16_t checksum = 0;

    for (size_t i = 0; i < 18; ++i)
    {
        checksum += packet[i];
    }

    return static_cast<uint8_t>(checksum & 0xFF);
}


bool KellyProtocol::validatePacket(
    const uint8_t* packet,
    uint8_t expectedCommand
) const
{
    // Correct command echo
    if (packet[0] != expectedCommand)
    {
        return false;
    }

    // Monitor response must contain 16 data bytes
    if (packet[1] != 0x10)
    {
        return false;
    }

    // Checksum
    uint8_t calculated = calculateChecksum(packet);

    if (calculated != packet[18])
    {
        return false;
    }

    return true;
}


bool KellyProtocol::testPacket(
    const uint8_t* packet,
    uint8_t* data
)
{
    if (packet == nullptr || data == nullptr)
    {
        return false;
    }

    if (!validatePacket(packet, packet[0]))
    {
        return false;
    }

    for (size_t i = 0; i < DATA_SIZE; ++i)
    {
        data[i] = packet[i + 2];
    }

    return true;
}