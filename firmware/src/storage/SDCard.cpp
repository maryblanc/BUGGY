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
    
    // czy te sciezki sie nie wywala na windowsie?
    if (!SD.exists("/LOGS"))
    {
        SD.mkdir("/LOGS");
    }

    File file = SD.open(filePath, FILE_APPEND);
    // klasa File ma mechanizm ktory sprawdza czy zostal poprawnie otwarty
    if (!file)
    {
        return false;
    }

    file.println(text);
    file.close();

    return true;

}
bool SDCard::exists(const String& path)
{
    return SD.exists(path);
}