#pragma once

#include <stdint.h>

#include "models/TelemetryFrame.h"

class KellyParser
{
public:
    TelemetryFrame parse(const uint8_t monitor1[16],
                         const uint8_t monitor2[16]);

private:
    float rawToPercent(uint8_t value);

    SwitchState parseSwitch(uint8_t value);
    Direction parseDirection(uint8_t value);
    HallState parseHall(uint8_t value);
};