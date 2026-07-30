#pragma once
#include "models/TelemetryFrame.h"

class KellyController
{
public:
    bool begin();

    TelemetryFrame readTelemetry();
};
