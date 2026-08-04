#include "rtc/RTCClock.h"

#include "Config.h"

RTCClock::RTCClock()
    : rtc(
        RTC_RST_PIN,
        RTC_CLK_PIN,
        RTC_DAT_PIN
    )
{
}

bool RTCClock::begin()
{
    rtc.init();

    if (rtc.isHalted())
    {
        rtc.start();
    }

    return true;
}

DateTime RTCClock::now()
{
    Ds1302::DateTime rtcDateTime;
    rtc.getDateTime(&rtcDateTime);

    DateTime dateTime;

    dateTime.year   = 2000 + rtcDateTime.year;
    dateTime.month  = rtcDateTime.month;
    dateTime.day    = rtcDateTime.day;

    dateTime.hour   = rtcDateTime.hour;
    dateTime.minute = rtcDateTime.minute;
    dateTime.second = rtcDateTime.second;

    return dateTime;
}

void RTCClock::setDateTime(const DateTime& dateTime)
{
    Ds1302::DateTime rtcDateTime;

    rtcDateTime.year   = dateTime.year % 100; // Store only last two digits of the year
    rtcDateTime.month  = dateTime.month;
    rtcDateTime.day    = dateTime.day;

    rtcDateTime.hour   = dateTime.hour;
    rtcDateTime.minute = dateTime.minute;
    rtcDateTime.second = dateTime.second;

    rtcDateTime.dow = Ds1302::DOW_MON; // Set day of the week to Monday (1) as a default value

    rtc.setDateTime(&rtcDateTime);
}