
#include <unity.h>
#include <stdint.h>

#include "../../src/controllers/KellySimulator.cpp"
#include "../../src/controllers/KellyParser.cpp"

void setUp() {}
void tearDown() {}

void test_simulator_data()
{
    KellySimulator simulator;
    KellyParser parser;

    uint8_t monitor1[16] = {};
    uint8_t monitor2[16] = {};

    TEST_ASSERT_TRUE(simulator.readMonitor1(monitor1));
    TEST_ASSERT_TRUE(simulator.readMonitor2(monitor2));

    // Expected simulator error flags: 17
    TEST_ASSERT_EQUAL_HEX8(0x00, monitor2[0]);
    TEST_ASSERT_EQUAL_HEX8(0x11, monitor2[1]);

    // RPM is stored in big-endian order.
    uint16_t rpm =
        (static_cast<uint16_t>(monitor2[2]) << 8) |
        monitor2[3];

    TEST_ASSERT_TRUE(rpm <= 5000);

    // Parse simulator output.
    TelemetryFrame frame =
        parser.parse(monitor1, monitor2);

    TEST_ASSERT_EQUAL_UINT16(17, frame.errorFlags);
    TEST_ASSERT_EQUAL_UINT16(rpm, frame.motorRPM);

    TEST_ASSERT_EQUAL_UINT8(120, frame.throttleRaw);
    TEST_ASSERT_EQUAL_UINT8(50, frame.brakeRaw);
}

void test_rpm_changes()
{
    KellySimulator simulator;

    uint8_t first[16] = {};
    uint8_t second[16] = {};

    TEST_ASSERT_TRUE(simulator.readMonitor2(first));
    TEST_ASSERT_TRUE(simulator.readMonitor2(second));

    uint16_t rpm1 =
        (static_cast<uint16_t>(first[2]) << 8) |
        first[3];

    uint16_t rpm2 =
        (static_cast<uint16_t>(second[2]) << 8) |
        second[3];

    TEST_ASSERT_EQUAL_UINT16(rpm1 + 100, rpm2);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_simulator_data);
    RUN_TEST(test_rpm_changes);

    return UNITY_END();
}
