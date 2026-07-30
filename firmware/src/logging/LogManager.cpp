#include "logging/LogManager.h"

LogManager::LogManager(SDCard& sd)
    : sd(sd)
{
}

String LogManager::generateLogFilePath()
{
    const String directory = "/LOGS";

    int fileNumber = 1;

    while (true)
    {
        String filePath = directory + "/telemetry_";

        if (fileNumber < 10)
        {
            filePath += "00";
        }
        else if (fileNumber < 100)
        {
            filePath += "0";
        }

        filePath += String(fileNumber);
        filePath += ".csv";

        if (!sd.exists(filePath))
        {
            return filePath;
        }

        fileNumber++;
    }
}