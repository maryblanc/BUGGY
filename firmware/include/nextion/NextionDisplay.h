#pragma once

#include <Arduino.h>
#include "Config.h"

class NextionDisplay
{
public:
    void begin();
    void setRPM(uint16_t rpm);

private:
    HardwareSerial& nextion = Serial2;

    void sendCommand(const char* command);
};