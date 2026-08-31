#include <Arduino.h>
#include "Config.h"

HardwareSerial nextion(2);

void setup()
{
    // USB ↔ PC
    Serial.begin(115200);

    // UART2 ↔ Nextion
    nextion.begin(
        NEXTION_BAUDRATE,
        SERIAL_8N1,
        NEXTION_RX_PIN,
        NEXTION_TX_PIN
    );

    delay(500);

    Serial.println("Nextion bridge ready");
}

void loop()
{
    // PC -> Nextion
    while (Serial.available())
    {
        nextion.write(Serial.read());
    }

    // Nextion -> PC
    while (nextion.available())
    {
        Serial.write(nextion.read());
    }
}