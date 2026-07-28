#include "models/TelemetryFrame.h"
#include <SPI.h>
#include <SD.h>


class CSVLogger
{
public:
    bool begin();

    bool write(const TelemetryFrame& frame);
};