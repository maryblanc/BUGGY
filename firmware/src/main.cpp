#include <Arduino.h>
#include <SPI.h>
#include <SD.h>


#include "Config.h"
#include "storage/SDCard.h"
#include "logging/CSVLogger.h"
#include "logging/LogManager.h"
#include "controllers/KellyController.h"
#include "rtc/RTCClock.h"

RTCClock rtc;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    // ---------- RTC ----------
    
    rtc.begin();

    
    // ======================================================
// RTC initialization
// Uncomment ONLY when setting the RTC for the first time
// or after replacing the RTC battery.
// ======================================================

// DateTime dt;

// dt.year   = 2026;
// dt.month  = 8;
// dt.day    = 4;
// dt.hour   = 14;
// dt.minute = 35;
// dt.second = 0;

// rtc.setDateTime(dt);
    // ---------- SD ----------
    SDCard sd;

    if (!sd.begin())
    {
        Serial.println("Failed to initialize SD card");
        return;
    }

    Serial.println("SD initialized");

    LogManager logManager(sd);

    String filePath = logManager.generateLogFilePath();
    CSVLogger logger(sd, filePath);

    KellyController controller;
    controller.begin();

    if (!logger.begin())
    {
        Serial.println("Failed to create telemetry.csv.");
        return;
    }

    Serial.println("CSV logger ready.");

    const unsigned long LOGGING_TIME_MS = 10000;
    unsigned long startTime = millis();

    while (millis() - startTime < LOGGING_TIME_MS)
    {
        TelemetryFrame frame = controller.readTelemetry();
        frame.timestamp = rtc.now(); // Set the timestamp to the current RTC time
        Serial.println(frame.timestamp.toString());

        

        // ---------- Logger ----------
        if (logger.write(frame))
        {
            Serial.println("Telemetry saved");
        }
        else
        {
            Serial.println("Failed to write telemetry");
        }

        Serial.print("RPM: ");
        Serial.println(frame.motorRPM);

        delay(100);
    }

    Serial.println("Logging finished.");
}

void loop()
{
}