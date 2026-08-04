#pragma once

#include "UARTPacket.h"
#include "models/TelemetryFrame.h"
class UARTPacketBuilder
{
public:
    UARTPacket build(const TelemetryFrame& frame) const;

private:
    void buildMonitor1(UARTPacket& packet, const TelemetryFrame& frame) const;

    void buildMonitor2(UARTPacket& packet, const TelemetryFrame& frame) const;
};