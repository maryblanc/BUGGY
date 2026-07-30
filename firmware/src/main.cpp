#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include "Config.h"
#include "storage/SDCard.h"
#include "logging/CSVLogger.h"
#include "controllers/KellyController.h"

void setup()
{
    Serial.begin(115200);
    delay(1000);

    SDCard sd;

    if (!sd.begin())
    {
        Serial.println("Failed to initialize SD card");
        return;
    }

    Serial.println("SD initialized");
    // Tworze logger i daje mu karte sd
    //CSVLogger logger(sd);
    CSVLogger logger(sd);
    KellyController controller;

    controller.begin();

    if (!logger.begin())
    {
        Serial.println("Failed to crerate telemetry.csv.");
        return;
    }
    Serial.println("CSV logger ready.");

    while (true)
    {
        TelemetryFrame frame = controller.readTelemetry();

        if (logger.write(frame))
        {
            Serial.println("Telemetry saved");
        }
        else
        {
            Serial.println("Failed to write telemetry");
        }
        

        Serial.println(frame.motorRPM);

        delay(100);
    }
}

void loop()
{
}