
#include <unity.h>
#include <cstdint>

// Korzystamy z prawdziwego kodu projektu.
#include "../../src/controllers/KellyParser.cpp"
#include "../../src/protocol/UARTPacketBuilder.cpp"

void setUp() {}
void tearDown() {}

void test_monitor2_encoding()
{
    TelemetryFrame input{};

    input.errorFlags = 0x1234;
    input.motorRPM = 2500;

    UARTPacketBuilder builder;
    UARTPacket packet = builder.build(input);

    TEST_ASSERT_EQUAL_HEX8(0x12, packet.monitor2[0]);
    TEST_ASSERT_EQUAL_HEX8(0x34, packet.monitor2[1]);

    TEST_ASSERT_EQUAL_HEX8(0x09, packet.monitor2[2]);
    TEST_ASSERT_EQUAL_HEX8(0xC4, packet.monitor2[3]);

    for (int i = 4; i < 16; ++i)
    {
        TEST_ASSERT_EQUAL_UINT8(0, packet.monitor2[i]);
    }
}

void test_monitor2_decoding()
{
    TelemetryFrame input{};

    input.errorFlags = 0x1234;
    input.motorRPM = 2500;
    input.throttleRaw = 255;
    input.brakeRaw = 0;

    UARTPacketBuilder builder;
    UARTPacket packet = builder.build(input);

    KellyParser parser;
    TelemetryFrame result =
        parser.parse(packet.monitor1, packet.monitor2);

    TEST_ASSERT_EQUAL_HEX16(0x1234, result.errorFlags);
    TEST_ASSERT_EQUAL_UINT16(2500, result.motorRPM);

    TEST_ASSERT_EQUAL_UINT8(255, result.throttleRaw);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 100.0f,
                             result.throttlePercent);

    TEST_ASSERT_EQUAL_UINT8(0, result.brakeRaw);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f,
                             result.brakePercent);
}

void test_monitor2_zero_values()
{
    TelemetryFrame input{};

    UARTPacketBuilder builder;
    UARTPacket packet = builder.build(input);

    KellyParser parser;
    TelemetryFrame result =
        parser.parse(packet.monitor1, packet.monitor2);

    TEST_ASSERT_EQUAL_UINT16(0, result.errorFlags);
    TEST_ASSERT_EQUAL_UINT16(0, result.motorRPM);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_monitor2_encoding);
    RUN_TEST(test_monitor2_decoding);
    RUN_TEST(test_monitor2_zero_values);

    return UNITY_END();
}
