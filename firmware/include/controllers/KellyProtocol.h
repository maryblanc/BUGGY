#pragma once
#include <cstdint>

class KellyProtocol
{
public:
    bool begin();

    bool readMonitor1(uint8_t* data);
    bool readMonitor2(uint8_t* data);
    bool readMonitor3(uint8_t* data);

private:
};