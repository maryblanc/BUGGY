#pragma once
// pragma zapobiega wielokrotnemu wczytaniu pliku
#include <cstdint>

enum class Direction{
    Forward,
    Reverse,
    Unknown,
    NotRecognized
};

enum class HallState{
    Low,
    High,
    Unknown,
    NotRecognized
};

enum class SwitchState{
    Off,
    On,
    Unknown,
    NotRecognized
};

struct TelemetryFrame{

    uint32_t timestamp;

    // Analog values
    uint8_t throttleRaw;
    uint8_t brakeRaw;

    float throttlePercent;
    float brakePercent;

    float batteryVoltage;

    float motorTemperature;
    float controllerTemperature;

    uint16_t motorRPM;
    float phaseCurrent;

    // Switches
    SwitchState brakeSwitch;
    SwitchState footSwitch;
    SwitchState forwardSwitch;
    SwitchState reverseSwitch;

    Direction setDirection;
    Direction actualDirection;

    HallState hallA;
    HallState hallB;
    HallState hallC;

    uint16_t errorFlags;
};