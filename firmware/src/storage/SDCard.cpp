#include "storage/SDCard.h"
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include "Config.h"

bool SDCard::begin()
{
    SPI.begin(
        SD_SCLK_PIN,
        SD_MISO_PIN,
        SD_MOSI_PIN,
        SD_CS_PIN
    );

    if (!SD.begin(SD_CS_PIN))
    {
        Serial.println("SD initialization failed!");
        return false;
    }

    return true;
}
// To jest implementacja funkcji writeLine, którą wcześniej zadeklarowaliśmy w klasie SDCard
bool SDCard::writeLine(String filePath, String text)
{    
    // FILE_WRITE  - otworzyc do zapisu (jesli nie istnieje, utworz go)
    File file = SD.open(filePath, FILE_WRITE);

    // klasa File ma mechanizm ktory sprawdza czy zostal poprawnie otwarty
    if (!file)
    {
        return false;
    }

    file.println(text);
    file.close();

    return true;

}

/* 
File file = SD.open("/LOGS/test.txt", FILE_WRITE);

    if (file)
    {
        file - obiekt, println - funkcja biblioteki SD
        czyli wszędzie jest obiekt.funkcja();
        file.println("Hello BUGGY!");
        file.close();

        Serial.println("File written.");
    }
    else
    {
        Serial.println("Cannot open file.");
    }
*/