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
    // Wyzeruj wszystkie bajty pakietu przed wpisaniem danych
    memset(data, 0, 16);

    // Symuluj rozpędzanie silnika
    simulatedRPM += 100;

    if (simulatedRPM > 5000)
    {
        simulatedRPM = 0;
    }

    uint16_t current = 321;
    uint16_t errors = 17;

    data[0] = current >> 8;
    data[1] = current & 0xFF;

    data[2] = simulatedRPM >> 8;
    data[3] = simulatedRPM & 0xFF;

    data[4] = errors >> 8;
    data[5] = errors & 0xFF;
    return true;
}