
#include <unity.h>
#include <stdint.h>

#include "controllers/KellyTelemetryAssembler.h"

// Real project implementations
#include "../../src/controllers/KellyParser.cpp"
#include "../../src/controllers/KellySimulator.cpp"
#include "../../src/protocol/KellyFrameCodec.cpp"
#include "../../src/protocol/KellyFrameReceiver.cpp"

void setUp() {}
void tearDown() {}

// Simulate a controller response passing through
// the real frame codec and frame receiver.
bool receiveSimulated(
    uint8_t command,
    const uint8_t payload[16],
    uint8_t output[16]
)
{
    uint8_t packet[19] = {};

    if (!KellyFrameCodec::buildTestResponse(
            command, payload, packet))
    {
        return false;
    }

    KellyFrameReceiver receiver(command);
    bool complete = false;

    for (uint8_t byte : packet)
    {
        complete = receiver.push(byte);
    }

    return complete && receiver.getPayload(output);
}

void test_both_monitors_valid()
{
    KellySimulator simulator;
    KellyParser parser;

    uint8_t raw1[16] = {};
    uint8_t raw2[16] = {};
    uint8_t data1[16] = {};
    uint8_t data2[16] = {};

    TEST_ASSERT_TRUE(simulator.readMonitor1(raw1));
    TEST_ASSERT_TRUE(simulator.readMonitor2(raw2));

    bool ok1 = receiveSimulated(0x3A, raw1, data1);
    bool ok2 = receiveSimulated(0x3B, raw2, data2);

    TelemetryFrame frame{};

    TEST_ASSERT_TRUE(
        KellyTelemetryAssembler::assemble(
            ok1, data1, ok2, data2, parser, frame)
    );

    TEST_ASSERT_EQUAL_UINT8(120, frame.throttleRaw);
    TEST_ASSERT_EQUAL_UINT8(50, frame.brakeRaw);
    TEST_ASSERT_EQUAL_UINT16(17, frame.errorFlags);
}

void test_monitor1_failed()
{
    KellyParser parser;
    TelemetryFrame frame{};
    frame.motorRPM = 9999;

    uint8_t data1[16] = {};
    uint8_t data2[16] = {};

    TEST_ASSERT_FALSE(
        KellyTelemetryAssembler::assemble(
            false, data1, true, data2, parser, frame)
    );

    TEST_ASSERT_EQUAL_UINT16(0, frame.motorRPM);
}

void test_monitor2_failed()
{
    KellyParser parser;
    TelemetryFrame frame{};
    frame.motorRPM = 9999;

    uint8_t data1[16] = {};
    uint8_t data2[16] = {};

    TEST_ASSERT_FALSE(
        KellyTelemetryAssembler::assemble(
            true, data1, false, data2, parser, frame)
    );

    TEST_ASSERT_EQUAL_UINT16(0, frame.motorRPM);
}

void test_both_monitors_failed()
{
    KellyParser parser;
    TelemetryFrame frame{};
    frame.errorFlags = 0xFFFF;
    frame.motorRPM = 9999;

    uint8_t data1[16] = {};
    uint8_t data2[16] = {};

    TEST_ASSERT_FALSE(
        KellyTelemetryAssembler::assemble(
            false, data1, false, data2, parser, frame)
    );

    TEST_ASSERT_EQUAL_UINT16(0, frame.motorRPM);
    TEST_ASSERT_EQUAL_UINT16(0, frame.errorFlags);
}

void test_null_payload()
{
    KellyParser parser;
    TelemetryFrame frame{};

    uint8_t data2[16] = {};

    TEST_ASSERT_FALSE(
        KellyTelemetryAssembler::assemble(
            true, nullptr, true, data2, parser, frame)
    );
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_both_monitors_valid);
    RUN_TEST(test_monitor1_failed);
    RUN_TEST(test_monitor2_failed);
    RUN_TEST(test_both_monitors_failed);
    RUN_TEST(test_null_payload);

    return UNITY_END();
}
