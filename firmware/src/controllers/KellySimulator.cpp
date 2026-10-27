#include "controllers/KellySimulator.h"
#include <cstring>

// USER_MONITOR1 (0x3A) - 16 bajtow
bool KellySimulator::readMonitor1(uint8_t* data)
{
    memset(data, 0, 16);

    data[0] = 120;   // throttle
    data[1] = 50;    // brake

    data[2] = 1;    // brake switch
    data[3] = 2;    // foot switch
    data[4] = 7;    // forward switch
    data[5] = 0;    // reverse switch

    data[6] = 2;    // hall A
    data[7] = 8;    // hall B
    data[8] = 0;    // hall C

    data[9] = 95;
    data[10] = 41;
    data[11] = 52;

    data[12] = 1;   // set direction
    data[13] = 9;   // actual direction

    return true;
}


bool KellySimulator::readMonitor2(uint8_t* data)
{
    if (data == nullptr)
    {
        return false;
    }

    // Clear all 16 payload bytes.
    memset(data, 0, 16);

    // Simulate motor acceleration.
    simulatedRPM += 100;

    if (simulatedRPM > 5000)
    {
        simulatedRPM = 0;
    }

    // Example diagnostic error flags.
    const uint16_t errors = 17;

    // Kelly USER_MONITOR2 (0x3B), RS232 v1.1
    // data[0-1]: error state, MSB first
    data[0] = static_cast<uint8_t>(errors >> 8);
    data[1] = static_cast<uint8_t>(errors & 0xFF);

    // data[2-3]: mechanical speed in RPM, MSB first
    data[2] = static_cast<uint8_t>(simulatedRPM >> 8);
    data[3] = static_cast<uint8_t>(simulatedRPM & 0xFF);

    // data[4-15]: not specified in supplied documentation.
    return true;
}
