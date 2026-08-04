#pragma once

#include <Ds1302.h>

#include "models/DateTime.h"

class RTCClock
{
public:
    RTCClock();

    bool begin();

    DateTime now();

    void setDateTime(const DateTime& dateTime);

private:
    Ds1302 rtc;
};