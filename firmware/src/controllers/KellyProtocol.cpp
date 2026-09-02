#include "controllers/KellyProtocol.h"
#include "Config.h"

bool KellyProtocol::begin()
{
    Serial.println("[KELLY] Initializing UART1...");
    Serial.print("[KELLY] Baud: ");
    Serial.println(UART_BAUDRATE);

    Serial.print("[KELLY] RX pin: ");
    Serial.println(KELLY_RX_PIN);

    Serial.print("[KELLY] TX pin: ");
    Serial.println(KELLY_TX_PIN);

    Serial1.begin(
        UART_BAUDRATE,
        SERIAL_8N1,
        KELLY_RX_PIN,
        KELLY_TX_PIN
    );

    delay(100);

    Serial.println("[KELLY] UART1 initialized");

    return true;
}

bool KellyProtocol::readMonitor1(uint8_t* data)
{
    Serial.println("[KELLY] Requesting MONITOR 1 (0x3A)");

    return readMonitor(MONITOR1_CMD, data);
}

bool KellyProtocol::readMonitor2(uint8_t* data)
{
    Serial.println("[KELLY] Requesting MONITOR 2 (0x3B)");

    return readMonitor(MONITOR2_CMD, data);
}

bool KellyProtocol::readMonitor3(uint8_t* data)
{
    Serial.println("[KELLY] Requesting MONITOR 3 (0x3C)");

    return readMonitor(MONITOR3_CMD, data);
}

bool KellyProtocol::readMonitor(
    uint8_t command,
    uint8_t* data
)
{
    if (data == nullptr)
    {
        Serial.println("[KELLY] ERROR: data pointer is null");
        return false;
    }

    for (uint8_t attempt = 0; attempt <= MAX_RETRIES; ++attempt)
    {
        Serial.print("[KELLY] Attempt ");
        Serial.print(attempt + 1);
        Serial.print("/");
        Serial.println(MAX_RETRIES + 1);

        Serial.println("[KELLY] Draining RX buffer...");

        drainReceiveBuffer();

        Serial.println("[KELLY] Sending command...");

        if (!sendCommand(command))
        {
            Serial.println("[KELLY] ERROR: failed to send command");
            continue;
        }

        Serial.println("[KELLY] Command sent");
        Serial.println("[KELLY] Waiting for response...");

        if (receivePacket(command, data))
        {
            Serial.println("[KELLY] Response received and VALID");
            return true;
        }

        Serial.println("[KELLY] Response INVALID or TIMEOUT");
    }

    Serial.println("[KELLY] FAILED: no valid response");

    return false;
}

void KellyProtocol::drainReceiveBuffer()
{
    size_t discarded = 0;

    while (Serial1.available() > 0)
    {
        Serial1.read();
        discarded++;
    }

    if (discarded > 0)
    {
        Serial.print("[KELLY] Discarded stale bytes: ");
        Serial.println(discarded);
    }
}

bool KellyProtocol::sendCommand(uint8_t command)
{
    uint8_t packet[3];

    packet[0] = command;
    packet[1] = 0x00;
    packet[2] = command;

    Serial.print("[KELLY] TX: ");

    for (size_t i = 0; i < sizeof(packet); ++i)
    {
        if (packet[i] < 0x10)
        {
            Serial.print("0");
        }

        Serial.print(packet[i], HEX);
        Serial.print(" ");
    }

    Serial.println();

    size_t written = Serial1.write(packet, sizeof(packet));

    Serial1.flush();

    Serial.print("[KELLY] Bytes written: ");
    Serial.println(written);

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
        if (Serial1.available() > 0)
        {
            packet[received] = Serial1.read();
            received++;
        }

        if (millis() - startTime >= RESPONSE_TIMEOUT_MS)
        {
            Serial.print("[KELLY] TIMEOUT: received ");
            Serial.print(received);
            Serial.print("/");
            Serial.print(PACKET_SIZE);
            Serial.println(" bytes");

            if (received > 0)
            {
                Serial.print("[KELLY] Partial RX: ");

                for (size_t i = 0; i < received; ++i)
                {
                    if (packet[i] < 0x10)
                    {
                        Serial.print("0");
                    }

                    Serial.print(packet[i], HEX);
                    Serial.print(" ");
                }

                Serial.println();
            }

            return false;
        }
    }

    Serial.print("[KELLY] RX: ");

    for (size_t i = 0; i < PACKET_SIZE; ++i)
    {
        if (packet[i] < 0x10)
        {
            Serial.print("0");
        }

        Serial.print(packet[i], HEX);
        Serial.print(" ");
    }

    Serial.println();

    if (packet[0] != expectedCommand)
    {
        Serial.print("[KELLY] ERROR: wrong command echo. Expected 0x");
        Serial.print(expectedCommand, HEX);
        Serial.print(", received 0x");
        Serial.println(packet[0], HEX);

        return false;
    }

    Serial.println("[KELLY] Command echo: OK");

    if (packet[1] != 0x10)
    {
        Serial.print("[KELLY] ERROR: wrong packet length byte: 0x");
        Serial.println(packet[1], HEX);

        return false;
    }

    Serial.println("[KELLY] Packet length: OK (16 bytes)");

    uint8_t calculated = calculateChecksum(packet);

    Serial.print("[KELLY] Checksum calculated: 0x");

    if (calculated < 0x10)
    {
        Serial.print("0");
    }

    Serial.println(calculated, HEX);

    Serial.print("[KELLY] Checksum received:   0x");

    if (packet[18] < 0x10)
    {
        Serial.print("0");
    }

    Serial.println(packet[18], HEX);

    if (calculated != packet[18])
    {
        Serial.println("[KELLY] ERROR: CHECKSUM INVALID");
        return false;
    }

    Serial.println("[KELLY] Checksum: OK");

    for (size_t i = 0; i < DATA_SIZE; ++i)
    {
        data[i] = packet[i + 2];
    }

    Serial.println("[KELLY] Payload extracted");

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
    if (packet[0] != expectedCommand)
    {
        return false;
    }

    if (packet[1] != 0x10)
    {
        return false;
    }

    uint8_t calculated = calculateChecksum(packet);

    return calculated == packet[18];
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