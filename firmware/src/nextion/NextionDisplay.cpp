#include "nextion/NextionDisplay.h"

void NextionDisplay::begin()
{
    nextion.begin(
        NEXTION_BAUDRATE,
        SERIAL_8N1,
        NEXTION_RX_PIN,
        NEXTION_TX_PIN
    );

    delay(500);
}

void NextionDisplay::sendCommand(const char* command)
{
    nextion.print(command);
    nextion.write(0xFF);
    nextion.write(0xFF);
    nextion.write(0xFF);
}

void NextionDisplay::setRPM(uint16_t rpm)
{
    char command[32];

    snprintf(
        command,
        sizeof(command),
        "tTitle.txt=\"%u\"",
        rpm
    );

    sendCommand(command);
}