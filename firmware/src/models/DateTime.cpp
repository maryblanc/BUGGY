#include "models/DateTime.h"

String DateTime::toString() const
{
    char buffer[20];

    snprintf(
        buffer,
        sizeof(buffer),
        "%04u-%02u-%02u %02u:%02u:%02u",
        year,
        month,
        day,
        hour,
        minute,
        second
    );

    return String(buffer);
}