#include <cstdint>

class KellySimulator
{
public:
    bool readMonitor1(uint8_t* data);
    bool readMonitor2(uint8_t* data);
private:
    // wartość uzywana do symulacji
    uint16_t simulatedRPM = 0;
};