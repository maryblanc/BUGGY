
#include <Arduino.h>

#include "controllers/KellyController.h"
#include "models/TelemetryFrame.h"

// Main Kelly controller interface
KellyController controller;

// Telemetry polling interval
constexpr unsigned long POLL_INTERVAL_MS = 1000;

unsigned long lastPoll = 0;

void printTelemetry(const TelemetryFrame& frame)
{
    Serial.println();
    Serial.println("====== KELLY TELEMETRY ======");

    Serial.print("Motor RPM: ");
    Serial.println(frame.motorRPM);

    Serial.print("Throttle raw: ");
    Serial.println(frame.throttleRaw);

    Serial.print("Throttle %: ");
    Serial.println(frame.throttlePercent, 1);

    Serial.print("Brake raw: ");
    Serial.println(frame.brakeRaw);

    Serial.print("Brake %: ");
    Serial.println(frame.brakePercent, 1);

    Serial.print("Error flags: 0x");
    Serial.println(frame.errorFlags, HEX);

    // Values below are raw A/D representations.
    // Physical units are not yet verified.
    Serial.print("Battery field (RAW, units unverified): ");
    Serial.println(frame.batteryVoltage);

    Serial.print("Motor temperature (RAW, units unverified): ");
    Serial.println(frame.motorTemperature);

    Serial.print("Controller temperature (RAW, units unverified: ");
    Serial.println(frame.controllerTemperature);

    Serial.println("=============================");
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("=============================");
    Serial.println("        BUGGY FIRMWARE");
    Serial.println("     Kelly KLS RS232 v1.1");
    Serial.println("=============================");

    if (!controller.begin())
    {
        Serial.println(
            "[BUGGY] Kelly UART initialization FAILED"
        );

        // Do not continue polling on init failure.
        while (true)
        {
            delay(1000);
        }
    }

    Serial.println("[BUGGY] Kelly UART initialized");
    Serial.println("[BUGGY] Starting telemetry polling");

    // Allow first read immediately.
    lastPoll = millis() - POLL_INTERVAL_MS;
}

void loop()
{
    const unsigned long now = millis();

    if (now - lastPoll < POLL_INTERVAL_MS)
    {
        delay(1);
        return;
    }

    lastPoll = now;

    Serial.println();
    Serial.println("[BUGGY] Reading Kelly telemetry...");

    const TelemetryFrame frame = controller.readTelemetry();
    
    Serial.print("[KELLY] MONITOR1: ");
    Serial.print(controller.getMonitor1Status().text());

    Serial.print(" | RX bytes: ");
    Serial.println(
        controller.getMonitor1Status().receivedBytes()
    );

    Serial.print("[KELLY] MONITOR2: ");
    Serial.print(controller.getMonitor2Status().text());

    Serial.print(" | RX bytes: ");
    Serial.println(
        controller.getMonitor2Status().receivedBytes()
    );


    if (!controller.isCommunicationOk())
    {
        Serial.println(
            "[BUGGY] NO VALID TELEMETRY"
        );

        Serial.println(
            "[BUGGY] Data unavailable - check connection"
        );

        // Do not display the zero-initialized failed read
        // as actual measurements.
        return;
    }

    Serial.println("[BUGGY] Communication OK");

    printTelemetry(frame);
}
