#include "models/TelemetryFrame.h"
#include "storage/SDCard.h"
#include <WString.h>



class CSVLogger
{
public:
    // konstruktor z referencja
    CSVLogger(SDCard& sd); // tu sd to parametr konstruktora
    bool begin();
    // REFERENCJA - Nie tworz kopii tylko korzystaj z tego samego obiektu
    // & oznacza referencje
    bool write(const TelemetryFrame& frame);

private:
    SDCard& sd; // tu sd to pole klasy
    const String filePath = "/LOGS/telemetry.csv";
    String telemetryToCSV(const TelemetryFrame& frame);
    // void - nie zwraca zadnych danych
    void append(String& line, const String& value);
    String switchStateToString(SwitchState state);
    String directionToString(Direction direction);
    String hallStateToString(HallState hall);
};


/*
Konstruktor

- specjalna funkcja klasy
- wywołuje się automatycznie przy tworzeniu obiektu
- ma taką samą nazwę jak klasa
- nie ma typu zwracanego (ani bool, ani void)
- może przyjmować parametry

Przykład:

class CSVLogger
{
public:
    CSVLogger(SDCard ...);
};

CSVLogger logger(sd);
*/