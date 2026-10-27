// STAN KONTROLERA

#pragma once
// pragma zapobiega wielokrotnemu wczytaniu pliku
#include <cstdint>
#include "DateTime.h"

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

    DateTime timestamp;

    // Analog values
    uint8_t throttleRaw;
    uint8_t brakeRaw;

    float throttlePercent;
    float brakePercent;
    // Raw values from MONITOR1.
    // Names are legacy names kept for compatibility.
    // These values are NOT verified physical units.

    float batteryVoltage; // RAW: monitor1[9]

    float motorTemperature;     // RAW: monitor1[10]
    float controllerTemperature;// RAW: monitor1[11]

    // Mechanical speed in RPM, from MONITOR2[2-3]
    // czy tu nie bylo problemu z sensorem?
    uint16_t motorRPM;
    // Not available from documented MONITOR2.
    // Currently set to 0 by the parser.
    // Do NOT interpret as a measured current.
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