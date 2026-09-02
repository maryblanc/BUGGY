#include <Arduino.h>
#include "Config.h"

HardwareSerial kelly(1);

void printHex(uint8_t b)
{
    if (b < 0x10)
        Serial.print("0");

    Serial.print(b, HEX);
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("================================");
    Serial.println("       KELLY RAW UART TEST");
    Serial.println("================================");
    Serial.println();

    Serial.println("[KELLY] Initializing UART1...");
    Serial.print("[KELLY] Baud: ");
    Serial.println(UART_BAUDRATE);

    Serial.print("[KELLY] RX pin: ");
    Serial.println(KELLY_RX_PIN);

    Serial.print("[KELLY] TX pin: ");
    Serial.println(KELLY_TX_PIN);

    kelly.begin(
        UART_BAUDRATE,
        SERIAL_8N1,
        KELLY_RX_PIN,
        KELLY_TX_PIN
    );

    Serial.println("[KELLY] UART1 initialized");
    Serial.println();

    delay(1000);
}

void loop()
{
    // --------------------------------------------------
    // Wyczyść stare dane
    // --------------------------------------------------

    while (kelly.available())
    {
        kelly.read();
    }

    // --------------------------------------------------
    // USER MONITOR 1
    //
    // 3A = command
    // 00 = no data
    // 3A = checksum
    // --------------------------------------------------

    uint8_t command[3] =
    {
        0x3A,
        0x00,
        0x3A
    };

    Serial.println("--------------------------------");
    Serial.println("[TX] Sending USER_MONITOR1");
    Serial.println("[TX] 3A 00 3A");

    size_t written = kelly.write(command, 3);
    kelly.flush();

    Serial.print("[TX] Bytes written: ");
    Serial.println(written);

    // --------------------------------------------------
    // Czekamy maksymalnie 500 ms
    // --------------------------------------------------

    Serial.println("[RX] Waiting for bytes...");

    unsigned long start = millis();

    uint8_t received = 0;

    while (millis() - start < 500)
    {
        while (kelly.available())
        {
            uint8_t b = kelly.read();

            Serial.print("[RX] byte ");
            Serial.print(received);
            Serial.print(": 0x");

            printHex(b);

            Serial.print("  (");
            Serial.print(b);
            Serial.println(")");

            received++;
        }
    }

    // --------------------------------------------------
    // Podsumowanie
    // --------------------------------------------------

    Serial.println();

    if (received == 0)
    {
        Serial.println("[RESULT] NOTHING RECEIVED");
    }
    else
    {
        Serial.print("[RESULT] RECEIVED ");
        Serial.print(received);
        Serial.println(" BYTES");
    }

    Serial.println();

    // Poczekaj przed kolejnym zapytaniem
    delay(2000);
}