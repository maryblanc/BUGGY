#include "controllers/KellyController.h"

bool KellyController::begin()
{
    return protocol.begin();
}

TelemetryFrame KellyController::readTelemetry()
{
    TelemetryFrame frame{};

    uint8_t monitor1[16] = {};
    uint8_t monitor2[16] = {};

    bool ok1 = protocol.readMonitor1(monitor1);
    bool ok2 = protocol.readMonitor2(monitor2);

    lastCommunicationOk = ok1 && ok2;

    if (!lastCommunicationOk)
    {
        return frame;
    }

    frame = parser.parse(monitor1, monitor2);

    return frame;
}

bool KellyController::isCommunicationOk() const
{
    return lastCommunicationOk;
}