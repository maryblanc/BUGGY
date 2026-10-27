
#include "protocol/UARTPacketBuilder.h"

UARTPacket UARTPacketBuilder::build(const TelemetryFrame& frame) const
{
    UARTPacket packet{};

    buildMonitor1(packet, frame);
    buildMonitor2(packet, frame);

    return packet;
}

void UARTPacketBuilder::buildMonitor1(
    UARTPacket& packet,
    const TelemetryFrame& frame
) const
{
    packet.monitor1[0] = frame.throttleRaw;
    packet.monitor1[1] = frame.brakeRaw;

    packet.monitor1[2] = static_cast<uint8_t>(frame.brakeSwitch);
    packet.monitor1[3] = static_cast<uint8_t>(frame.footSwitch);
    packet.monitor1[4] = static_cast<uint8_t>(frame.forwardSwitch);
    packet.monitor1[5] = static_cast<uint8_t>(frame.reverseSwitch);

    packet.monitor1[6] = static_cast<uint8_t>(frame.hallA);
    packet.monitor1[7] = static_cast<uint8_t>(frame.hallB);
    packet.monitor1[8] = static_cast<uint8_t>(frame.hallC);

    packet.monitor1[9]  = frame.batteryVoltage;
    packet.monitor1[10] = frame.motorTemperature;
    packet.monitor1[11] = frame.controllerTemperature;

    packet.monitor1[12] = static_cast<uint8_t>(frame.setDirection);
    packet.monitor1[13] = static_cast<uint8_t>(frame.actualDirection);
}

void UARTPacketBuilder::buildMonitor2(
    UARTPacket& packet,
    const TelemetryFrame& frame
) const
{
    // Kelly KLS RS232 v1.1 - USER_MONITOR2

    // Bytes 0-1: error flags (MSB, LSB)
    packet.monitor2[0] =
        static_cast<uint8_t>(frame.errorFlags >> 8);

    packet.monitor2[1] =
        static_cast<uint8_t>(frame.errorFlags & 0xFF);

    // Bytes 2-3: motor RPM (MSB, LSB)
    packet.monitor2[2] =
        static_cast<uint8_t>(frame.motorRPM >> 8);

    packet.monitor2[3] =
        static_cast<uint8_t>(frame.motorRPM & 0xFF);

    // Bytes 4-15 are unspecified in the supplied PDF.
    // They remain zero in simulator-generated data.
}
