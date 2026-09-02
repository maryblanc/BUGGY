#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include "Config.h"
#include "storage/SDCard.h"
#include "logging/CSVLogger.h"
#include "logging/LogManager.h"
#include "controllers/KellyController.h"
#include "controllers/KellyProtocol.h"
#include "rtc/RTCClock.h"
#include "protocol/UARTPacketBuilder.h"
#include "controllers/KellyParser.h"
#include "nextion/NextionDisplay.h"

RTCClock rtc;
NextionDisplay display;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("===============================");
    Serial.println();

    // ---------- KELLY PACKET -> TELEMETRY TEST ----------

    // --------------------------------------------------
    // Test packet A = Kelly Monitor 1
    // --------------------------------------------------

    uint8_t testPacketA[19] =
    {
        0x3A,       // command
        0x10,       // length

        123,        // throttle
        45,         // brake

        1,          // brake switch
        2,          // foot switch
        1,          // forward switch
        0,          // reverse switch

        1,          // hall A
        0,          // hall B
        1,          // hall C

        96,         // battery voltage
        42,         // motor temperature
        51,         // controller temperature

        0,          // set direction
        0,          // actual direction

        0,          // unused
        0,          // unused

        0           // checksum
    };


    // --------------------------------------------------
    // Test packet B = Kelly Monitor 2
    // --------------------------------------------------

    uint8_t testPacketB[19] =
{
    0x3B,       // command
    0x10,       // length

    0x01,       // phase current high byte
    0x41,       // phase current low byte = 321

    0x10,       // RPM high byte
    0xE1,       // RPM low byte = 4321

    0x00,       // error flags high byte
    0x06,       // error flags low byte = 6

    0x00,       // unused
    0x00,       // unused
    0x00,       // unused
    0x00,       // unused
    0x00,       // unused
    0x00,       // unused
    0x00,       // unused
    0x00,       // unused
    0x00,       // unused

    0x00        // checksum
};


    // --------------------------------------------------
    // Calculate checksums
    // --------------------------------------------------

    uint16_t checksumA = 0;

    for (int i = 0; i < 18; ++i)
    {
        checksumA += testPacketA[i];
    }

    testPacketA[18] = checksumA & 0xFF;


    uint16_t checksumB = 0;

    for (int i = 0; i < 18; ++i)
    {
        checksumB += testPacketB[i];
    }

    testPacketB[18] = checksumB & 0xFF;


    Serial.print("Packet B calculated checksum: ");
    Serial.println(testPacketB[18]);

    Serial.print("Packet B bytes: ");

    for (int i = 0; i < 19; ++i)
    {
        if (testPacketB[i] < 16)
        {
            Serial.print("0");
        }

        Serial.print(testPacketB[i], HEX);
        Serial.print(" ");
    }

    Serial.println();

    // --------------------------------------------------
    // Decode Kelly packets
    // --------------------------------------------------

    KellyProtocol protocol;
    KellyParser parser;

    uint8_t monitor1[16] = {};
    uint8_t monitor2[16] = {};

    bool packetAValid =
        protocol.testPacket(testPacketA, monitor1);

    bool packetBValid =
        protocol.testPacket(testPacketB, monitor2);


    // --------------------------------------------------
    // Expected telemetry
    // --------------------------------------------------

    TelemetryFrame expected{};

    expected.throttleRaw = 123;
    expected.brakeRaw = 45;

    expected.batteryVoltage = 96;
    expected.motorTemperature = 42;
    expected.controllerTemperature = 51;

    expected.motorRPM = 4321;
    expected.phaseCurrent = 321;
    expected.errorFlags = 0x0006;

    expected.brakeSwitch = SwitchState::On;
    expected.footSwitch = SwitchState::Unknown;
    expected.forwardSwitch = SwitchState::On;
    expected.reverseSwitch = SwitchState::Off;

    expected.hallA = HallState::High;
    expected.hallB = HallState::Low;
    expected.hallC = HallState::High;

    expected.setDirection = Direction::Forward;
    expected.actualDirection = Direction::Forward;


    // --------------------------------------------------
    // Parse monitor data into TelemetryFrame
    // --------------------------------------------------

    TelemetryFrame parsed =
        parser.parse(monitor1, monitor2);


    // --------------------------------------------------
    // Compare
    // --------------------------------------------------

    bool telemetryPassed = true;

    telemetryPassed &= packetAValid;
    telemetryPassed &= packetBValid;

    telemetryPassed &= parsed.throttleRaw == expected.throttleRaw;
    telemetryPassed &= parsed.brakeRaw == expected.brakeRaw;

    telemetryPassed &= parsed.batteryVoltage == expected.batteryVoltage;
    telemetryPassed &= parsed.motorTemperature == expected.motorTemperature;
    telemetryPassed &= parsed.controllerTemperature == expected.controllerTemperature;

    telemetryPassed &= parsed.motorRPM == expected.motorRPM;
    telemetryPassed &= parsed.phaseCurrent == expected.phaseCurrent;
    telemetryPassed &= parsed.errorFlags == expected.errorFlags;

    telemetryPassed &= parsed.brakeSwitch == expected.brakeSwitch;
    telemetryPassed &= parsed.footSwitch == expected.footSwitch;
    telemetryPassed &= parsed.forwardSwitch == expected.forwardSwitch;
    telemetryPassed &= parsed.reverseSwitch == expected.reverseSwitch;

    telemetryPassed &= parsed.hallA == expected.hallA;
    telemetryPassed &= parsed.hallB == expected.hallB;
    telemetryPassed &= parsed.hallC == expected.hallC;

    telemetryPassed &= parsed.setDirection == expected.setDirection;
    telemetryPassed &= parsed.actualDirection == expected.actualDirection;


    // --------------------------------------------------
    // Print result
    // --------------------------------------------------

    Serial.println();
    Serial.println("========== KELLY -> TELEMETRY TEST ==========");

    Serial.print("Packet A valid: ");
    Serial.println(packetAValid ? "YES" : "NO");

    Serial.print("Packet B valid: ");
    Serial.println(packetBValid ? "YES" : "NO");

    Serial.println();

    Serial.print("Throttle:        ");
    Serial.print(expected.throttleRaw);
    Serial.print(" -> ");
    Serial.println(parsed.throttleRaw);

    Serial.print("Brake:           ");
    Serial.print(expected.brakeRaw);
    Serial.print(" -> ");
    Serial.println(parsed.brakeRaw);

    Serial.print("Battery:         ");
    Serial.print(expected.batteryVoltage);
    Serial.print(" -> ");
    Serial.println(parsed.batteryVoltage);

    Serial.print("Motor temp:      ");
    Serial.print(expected.motorTemperature);
    Serial.print(" -> ");
    Serial.println(parsed.motorTemperature);

    Serial.print("Controller temp: ");
    Serial.print(expected.controllerTemperature);
    Serial.print(" -> ");
    Serial.println(parsed.controllerTemperature);

    Serial.print("RPM:             ");
    Serial.print(expected.motorRPM);
    Serial.print(" -> ");
    Serial.println(parsed.motorRPM);

    Serial.print("Phase current:   ");
    Serial.print(expected.phaseCurrent);
    Serial.print(" -> ");
    Serial.println(parsed.phaseCurrent);

    Serial.print("Error flags:     ");
    Serial.print(expected.errorFlags);
    Serial.print(" -> ");
    Serial.println(parsed.errorFlags);

    if (telemetryPassed)
    {
        Serial.println("RESULT: PASS");
    }
    else
    {
        Serial.println("RESULT: FAIL");
    }

    Serial.println("==============================================");
    Serial.println();

    // ---------- RTC ----------
    
    rtc.begin();

    
    // ======================================================
// RTC initialization
// Uncomment ONLY when setting the RTC for the first time
// or after replacing the RTC battery.
// ======================================================

// DateTime dt;

// dt.year   = 2026;
// dt.month  = 8;
// dt.day    = 4;
// dt.hour   = 14;
// dt.minute = 35;
// dt.second = 0;

// rtc.setDateTime(dt);
    // ---------- SD ----------

    // DISABLED FOR NEXTION TESTING
    /*
    SDCard sd;

    if (!sd.begin())
    {
        Serial.println("Failed to initialize SD card");
        return;
    }

    Serial.println("SD initialized");

    LogManager logManager(sd);

    String filePath = logManager.generateLogFilePath();
    CSVLogger logger(sd, filePath);

    KellyController controller;
    controller.begin();
    display.begin();

    if (!logger.begin())
    {
        Serial.println("Failed to create telemetry.csv.");
        return;
    }

    Serial.println("CSV logger ready."); */

    KellyController controller;
    controller.begin();
    display.begin();

    const unsigned long LOGGING_TIME_MS = 10000;
    unsigned long startTime = millis();

    while (millis() - startTime < LOGGING_TIME_MS)
    {
        TelemetryFrame frame = controller.readTelemetry();
        frame.timestamp = rtc.now(); // Set the timestamp to the current RTC time

        display.setRPM(frame.motorRPM);
        Serial.println(frame.timestamp.toString());

        
/*
        // ---------- Logger ----------
        if (logger.write(frame))
        {
            Serial.println("Telemetry saved");
        }
        else
        {
            Serial.println("Failed to write telemetry");
        } */

        Serial.print("RPM: ");
        Serial.println(frame.motorRPM);

        delay(100);
    }

    Serial.println("Logging finished.");
}

void loop()
{
}