#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include "Config.h"

void setup()
{
    Serial.begin(115200);
    delay(1000);

    SPI.begin(
        SD_SCLK_PIN,
        SD_MISO_PIN,
        SD_MOSI_PIN,
        SD_CS_PIN
    );

    if (!SD.begin(SD_CS_PIN))
    {
        Serial.println("SD initialization failed!");
        return;
    }

    Serial.println("SD initialized.");

    File file = SD.open("/LOGS/test.txt", FILE_WRITE);

    if (file)
    {
        file.println("Hello BUGGY!");
        file.close();

        Serial.println("File written.");
    }
    else
    {
        Serial.println("Cannot open file.");
    }
}

void loop()
{
}