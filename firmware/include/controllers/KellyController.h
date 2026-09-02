#pragma once

#include "models/TelemetryFrame.h"
#include "controllers/KellyProtocol.h"
#include "controllers/KellyParser.h"

class KellyController
{
public:
    bool begin();
    TelemetryFrame readTelemetry();

    bool isCommunicationOk() const;

private:
    KellyProtocol protocol;
    KellyParser parser;

    bool lastCommunicationOk = false;
};