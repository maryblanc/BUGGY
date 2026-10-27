
#pragma once

#include "models/TelemetryFrame.h"
#include "controllers/KellyProtocol.h"
#include "controllers/KellyParser.h"
#include "protocol/KellyLinkStatus.h"

class KellyController
{
public:
    bool begin();

    TelemetryFrame readTelemetry();

    bool isCommunicationOk() const;

    const KellyLinkStatus& getMonitor1Status() const;
    const KellyLinkStatus& getMonitor2Status() const;

private:
    KellyProtocol protocol;
    KellyParser parser;

    bool lastCommunicationOk = false;

    // Separate status for each read command.
    KellyLinkStatus monitor1Status;
    KellyLinkStatus monitor2Status;
};
