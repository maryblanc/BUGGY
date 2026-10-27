
#include <unity.h>
#include <stdint.h>

#include "../../src/protocol/KellyFrameCodec.cpp"
#include "../../src/protocol/KellyFrameReceiver.cpp"

void setUp() {}
void tearDown() {}

void makeFrame(uint8_t out[19])
{
    uint8_t data[16] = {};

    data[0] = 0x12;
    data[1] = 0x34;
    data[2] = 0x09;
    data[3] = 0xC4;

    TEST_ASSERT_TRUE(
        KellyFrameCodec::buildTestResponse(
            0x3B, data, out
        )
    );
}

void test_complete_frame()
{
    uint8_t packet[19];
    makeFrame(packet);

    KellyFrameReceiver receiver(0x3B);
    bool result = false;

    for (uint8_t b : packet)
    {
        result = receiver.push(b);
    }

    TEST_ASSERT_TRUE(result);

    uint8_t data[16] = {};
    TEST_ASSERT_TRUE(receiver.getPayload(data));
    TEST_ASSERT_EQUAL_HEX8(0x12, data[0]);
    TEST_ASSERT_EQUAL_HEX8(0x34, data[1]);
    TEST_ASSERT_EQUAL_HEX8(0x09, data[2]);
    TEST_ASSERT_EQUAL_HEX8(0xC4, data[3]);
}

void test_noise_before_frame()
{
    uint8_t packet[19];
    makeFrame(packet);

    KellyFrameReceiver receiver(0x3B);

    TEST_ASSERT_FALSE(receiver.push(0xFF));
    TEST_ASSERT_FALSE(receiver.push(0x00));
    TEST_ASSERT_FALSE(receiver.push(0xAA));

    bool result = false;

    for (uint8_t b : packet)
    {
        result = receiver.push(b);
    }

    TEST_ASSERT_TRUE(result);
}

void test_incomplete_frame()
{
    uint8_t packet[19];
    makeFrame(packet);

    KellyFrameReceiver receiver(0x3B);

    for (size_t i = 0; i < 10; ++i)
    {
        TEST_ASSERT_FALSE(receiver.push(packet[i]));
    }

    uint8_t data[16] = {};
    TEST_ASSERT_FALSE(receiver.getPayload(data));
}

void test_corrupted_then_valid()
{
    uint8_t packet[19];
    makeFrame(packet);

    KellyFrameReceiver receiver(0x3B);

    uint8_t damaged[19];
    for (size_t i = 0; i < 19; ++i)
    {
        damaged[i] = packet[i];
    }

    damaged[5] ^= 0x01;

    for (uint8_t b : damaged)
    {
        TEST_ASSERT_FALSE(receiver.push(b));
    }

    bool result = false;

    for (uint8_t b : packet)
    {
        result = receiver.push(b);
    }

    TEST_ASSERT_TRUE(result);
}

void test_wrong_command_then_valid()
{
    uint8_t packet[19];
    makeFrame(packet);

    KellyFrameReceiver receiver(0x3B);

    uint8_t wrong[19] = {};
    uint8_t data[16] = {};

    TEST_ASSERT_TRUE(
        KellyFrameCodec::buildTestResponse(
            0x3A, data, wrong
        )
    );

    for (uint8_t b : wrong)
    {
        TEST_ASSERT_FALSE(receiver.push(b));
    }

    bool result = false;

    for (uint8_t b : packet)
    {
        result = receiver.push(b);
    }

    TEST_ASSERT_TRUE(result);
}

void test_reset()
{
    uint8_t packet[19];
    makeFrame(packet);

    KellyFrameReceiver receiver(0x3B);

    for (uint8_t b : packet)
    {
        receiver.push(b);
    }

    uint8_t data[16] = {};
    TEST_ASSERT_TRUE(receiver.getPayload(data));

    receiver.reset();

    TEST_ASSERT_FALSE(receiver.getPayload(data));

    for (uint8_t b : packet)
    {
        receiver.push(b);
    }

    TEST_ASSERT_TRUE(receiver.getPayload(data));
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_complete_frame);
    RUN_TEST(test_noise_before_frame);
    RUN_TEST(test_incomplete_frame);
    RUN_TEST(test_corrupted_then_valid);
    RUN_TEST(test_wrong_command_then_valid);
    RUN_TEST(test_reset);

    return UNITY_END();
}
