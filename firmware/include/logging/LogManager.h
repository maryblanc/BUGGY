#include <storage/SDCard.h>
class LogManager
{
public:
    LogManager(SDCard& sd);

    String generateLogFilePath();

private:
    SDCard& sd;
    const String logDirectory = "/LOGS";
    const String filePrefix = "telemetry_";
};