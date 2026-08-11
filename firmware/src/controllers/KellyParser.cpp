#include "controllers/KellyParser.h"

TelemetryFrame KellyParser::parse(const uint8_t monitor1[16],
                                  const uint8_t monitor2[16])
{
    TelemetryFrame frame;
    frame.throttleRaw = monitor1[0];
    frame.throttlePercent = rawToPercent(frame.throttleRaw);
    frame.brakeRaw = monitor1[1];
    frame.brakePercent = rawToPercent(frame.brakeRaw);
    frame.batteryVoltage = monitor1[9];
    frame.motorTemperature = monitor1[10];
    frame.controllerTemperature = monitor1[11];

    // Kelly przesyła wartości większe niż 255 jako dwa bajty.
    // W formacie big-endian pierwszy bajt zawiera starszą część liczby,
    // a drugi młodszą. Przesunięcie << 8 umieszcza starszy bajt na właściwej
    // pozycji, a operator | łączy oba bajty w jedną liczbę 16-bitową.
    frame.motorRPM = (monitor2[2] << 8) | monitor2[3];
    frame.phaseCurrent = (monitor2[0] << 8) | monitor2[1];


    frame.errorFlags = (monitor2[4] << 8) | monitor2[5];

    // SWITCHE

    frame.brakeSwitch = parseSwitch(monitor1[2]);
    frame.footSwitch = parseSwitch(monitor1[3]);
    frame.forwardSwitch = parseSwitch(monitor1[4]);
    frame.reverseSwitch = parseSwitch(monitor1[5]);

    frame.hallA = parseHall(monitor1[6]);
    frame.hallB = parseHall(monitor1[7]);
    frame.hallC = parseHall(monitor1[8]);

    frame.setDirection = parseDirection(monitor1[12]);
    frame.actualDirection = parseDirection(monitor1[13]);

    return frame;
}

float KellyParser::rawToPercent(uint8_t value)
{
    return value * 100.0f / 255.0f;
}

SwitchState KellyParser::parseSwitch(uint8_t value)
{
    switch (value)
    {
        case 0:
            return SwitchState::Off;
        
        case 1: 
            return SwitchState::On;

        case 2:
            return SwitchState::Unknown;

        default:
            return SwitchState::NotRecognized;

    }
}


Direction KellyParser::parseDirection(uint8_t value)
{
    switch (value)
    {
        case 0:
            return Direction::Forward;

        case 1: 
            return Direction::Reverse;

        case 2:
            return Direction::Unknown;
        
        default:
            return Direction::NotRecognized;
    }
}

HallState KellyParser::parseHall(uint8_t value)
{
    switch (value)
    {
        case 0:
            return HallState::Low;

        case 1:
            return HallState::High;

        case 2:
            return HallState::Unknown;

        default:
            return HallState::NotRecognized;
    }
}