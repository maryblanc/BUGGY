#include "logging/CSVLogger.h"

CSVLogger::CSVLogger(SDCard& sd)
    : sd(sd)
{
}

bool CSVLogger::begin()
{
    return sd.writeLine(filePath, "timestamp,throttleRaw,throttlePercent,brakeRaw,brakePercent,batteryVoltage,motorTemperature,controllerTemperature,motorRPM,phaseCurrent,errorFlags,brakeSwitch,footSwitch,forwardSwitch,reverseSwitch,setDirection,actualDirection,hallA,hallB,hallC");
}

bool CSVLogger::write(const TelemetryFrame& frame)
{
   return sd.writeLine(filePath, telemetryToCSV(frame));
}

String CSVLogger::telemetryToCSV(const TelemetryFrame& frame)
{
    String line;

    append(line, String(frame.timestamp));
    append(line, String(frame.throttleRaw));
    append(line, String(frame.throttlePercent));
    append(line, String(frame.brakeRaw));
    append(line, String(frame.brakePercent));
    append(line, String(frame.batteryVoltage));
    append(line, String(frame.motorTemperature));
    append(line, String(frame.controllerTemperature));
    append(line, String(frame.motorRPM));
    append(line, String(frame.phaseCurrent));
    append(line, String(frame.errorFlags));
    append(line, switchStateToString(frame.brakeSwitch));
    append(line, switchStateToString(frame.footSwitch));
    append(line, switchStateToString(frame.forwardSwitch));
    append(line, switchStateToString(frame.reverseSwitch));
    append(line, directionToString(frame.setDirection));
    append(line, directionToString(frame.actualDirection));

    append(line, hallStateToString(frame.hallA));
    append(line, hallStateToString(frame.hallB));
    append(line, hallStateToString(frame.hallC));

    

    return line;
}

void CSVLogger::append(String& line, const String& value)
{
    if (!line.isEmpty())
    {
        line += ",";
    }
    line += value;
}

String CSVLogger::switchStateToString(SwitchState state)
{
    switch(state)
    {
        case SwitchState::Off:
            return "Off";

        case SwitchState::On:
            return "On";

        case SwitchState::Unknown:
            return "Unknown";
    }
    return "Not recognized";
}

String CSVLogger::directionToString(Direction direction)
{
    switch(direction)
    {
        case Direction::Forward:
            return "Forward";

        case Direction::Reverse:
            return "Reverse";

        case Direction::Unknown:
            return "Unknown";

    }
    return "Not recognized";
}

String CSVLogger::hallStateToString(HallState hall)
{
    switch(hall)
    {
        case HallState::Low:
            return "Low";

        case HallState::High:
            return "High";

        case HallState::Unknown:
            return "Unknown";

    }
    return "Not recognized";
}