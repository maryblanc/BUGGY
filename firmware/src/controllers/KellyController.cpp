#include "controllers/KellyController.h"
#include <esp32-hal.h>

bool KellyController::begin()
{
    return true;
}

TelemetryFrame KellyController::readTelemetry()
{
    TelemetryFrame frame;

    frame.timestamp = millis();
    frame.motorRPM = 2500;
    frame.batteryVoltage = 82.4;
    frame.throttlePercent = 37;

    return frame;
}