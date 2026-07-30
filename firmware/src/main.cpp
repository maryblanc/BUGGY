#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include "Config.h"
#include "storage/SDCard.h"
#include "logging/CSVLogger.h"

void setup()
{
    Serial.begin(115200);
    delay(1000);

    
    
    SDCard sd;
    
    if (!sd.begin())
    {
        return;
    }
    //// Tworze logger i daje mu karte sd
    //CSVLogger logger(sd);
    CSVLogger logger(sd);

    logger.begin();

    TelemetryFrame frame;

    frame.timestamp = 1234;
    frame.batteryVoltage = 48.7;
    frame.motorRPM = 3120;

    logger.write(frame);
}

void loop()
{
}