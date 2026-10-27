#include "controllers/KellyProtocol.h"
#include "Config.h"
#include "protocol/KellyFrameCodec.h"
#include "protocol/KellyFrameReceiver.h"

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
        linkStatus_.beginRequest();
        Serial.print("[KELLY] Attempt ");
        Serial.print(attempt + 1);
        Serial.print("/");
        Serial.println(MAX_RETRIES + 1);

        Serial.println("[KELLY] Draining RX buffer...");

        drainReceiveBuffer();

        Serial.println("[KELLY] Sending command...");

        if (!sendCommand(command))
        {
            linkStatus_.markSendError();
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
    uint8_t packet[KellyFrameCodec::REQUEST_SIZE] = {};

    // Build request according to Kelly KLS RS232 v1.1.
    if (!KellyFrameCodec::buildRequest(command, packet))
    {
        Serial.println("[KELLY] ERROR: unsupported command");
        return false;
    }

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
    if (data == nullptr)
    {
        Serial.println("[KELLY] ERROR: null data pointer");
        return false;
    }

    KellyFrameReceiver receiver(expectedCommand);

    const unsigned long startTime = millis();

    size_t receivedBytes = 0;

    Serial.println("[KELLY] Listening for RS232 response...");

    while (millis() - startTime < RESPONSE_TIMEOUT_MS)
    {
        while (Serial1.available() > 0)
        {
            int incoming = Serial1.read();

            if (incoming < 0)
            {
                continue;
            }

            uint8_t byte = static_cast<uint8_t>(incoming);
            receivedBytes++;
            linkStatus_.recordReceivedByte();

            // Pass each byte to the tested frame receiver.
            if (receiver.push(byte))
            {
                if (receiver.getPayload(data))
                {
                    linkStatus_.markSuccess();
                    Serial.println(
                        "[KELLY] VALID RESPONSE RECEIVED"
                    );

                    Serial.print("[KELLY] Bytes read: ");
                    Serial.println(receivedBytes);

                    Serial.println(
                        "[KELLY] Checksum and command: OK"
                    );

                    return true;
                }
            }
        }

        // Allow the ESP32 to handle other system tasks.
        delay(1);
    }
    linkStatus_.markTimeout();
    Serial.println("[KELLY] RESPONSE TIMEOUT");

    Serial.print("[KELLY] Total bytes received: ");
    Serial.println(receivedBytes);

    if (receivedBytes == 0)
    {
        Serial.println(
            "[KELLY] NO BYTES RECEIVED - check wiring, "
            "power supply and RS232 converter"
        );
    }
    else
    {
        Serial.println(
            "[KELLY] Bytes received, but no valid frame found"
        );
    }

    return false;
}




bool KellyProtocol::validatePacket(
    const uint8_t* packet,
    uint8_t expectedCommand
) const
{
    return KellyFrameCodec::validateResponse(
        packet,
        PACKET_SIZE,
        expectedCommand
    );
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