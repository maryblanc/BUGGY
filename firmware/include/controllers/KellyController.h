#pragma once
#include "models/TelemetryFrame.h"
#include "controllers/KellySimulator.h"
#include "controllers/KellyParser.h"

class KellyController
{
public:
    bool begin();

    TelemetryFrame readTelemetry();
private:
    KellySimulator simulator;
    
    KellyParser parser;
};
