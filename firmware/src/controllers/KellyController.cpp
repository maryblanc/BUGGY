
#include "controllers/KellyController.h"

bool KellyController::begin()
{
    lastCommunicationOk = false;

    return protocol.begin();
}

TelemetryFrame KellyController::readTelemetry()
{
    TelemetryFrame frame{};

    uint8_t monitor1[16] = {};
    uint8_t monitor2[16] = {};

    // Read MONITOR1 and save its status.
    bool ok1 = protocol.readMonitor1(monitor1);
    monitor1Status = protocol.getLinkStatus();

    // Read MONITOR2 and save its status.
    bool ok2 = protocol.readMonitor2(monitor2);
    monitor2Status = protocol.getLinkStatus();

    lastCommunicationOk = ok1 && ok2;

    if (!lastCommunicationOk)
    {
        // No complete telemetry available.
        return frame;
    }

    return parser.parse(monitor1, monitor2);
}

bool KellyController::isCommunicationOk() const
{
    return lastCommunicationOk;
}

const KellyLinkStatus&
KellyController::getMonitor1Status() const
{
    return monitor1Status;
}

const KellyLinkStatus&
KellyController::getMonitor2Status() const
{
    return monitor2Status;
}
