
#include "controllers/KellyController.h"
#include "controllers/KellyTelemetryAssembler.h"

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

    // Read MONITOR1 and preserve diagnostics.
    const bool ok1 = protocol.readMonitor1(monitor1);
    monitor1Status = protocol.getLinkStatus();

    // Read MONITOR2 and preserve diagnostics.
    const bool ok2 = protocol.readMonitor2(monitor2);
    monitor2Status = protocol.getLinkStatus();

    // Accept telemetry only if both reads succeeded.
    lastCommunicationOk = KellyTelemetryAssembler::assemble(
        ok1,
        monitor1,
        ok2,
        monitor2,
        parser,
        frame
    );

    return frame;
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
