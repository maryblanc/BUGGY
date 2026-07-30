#include "controllers/KellyController.h"
#include <esp32-hal.h>

bool KellyController::begin()
{
    return true;
}


TelemetryFrame KellyController::readTelemetry()
{
    TelemetryFrame frame;

    uint8_t monitor1[16];
    uint8_t monitor2[16];

    simulator.readMonitor1(monitor1);
    simulator.readMonitor2(monitor2);
    frame = parser.parse(monitor1, monitor2);

    frame.timestamp = millis();
    return frame;
}