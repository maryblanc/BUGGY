
#include <unity.h>
#include <stdint.h>

#include "../../src/protocol/KellyFrameCodec.cpp"

void setUp() {}
void tearDown() {}

// TEST 1: Three read requests
void test_read_commands()
{
    uint8_t request[3] = {};

    TEST_ASSERT_TRUE(
        KellyFrameCodec::buildRequest(0x1B, request)
    );
    TEST_ASSERT_EQUAL_HEX8(0x1B, request[0]);
    TEST_ASSERT_EQUAL_HEX8(0x00, request[1]);
    TEST_ASSERT_EQUAL_HEX8(0x1B, request[2]);

    TEST_ASSERT_TRUE(
        KellyFrameCodec::buildRequest(0x3A, request)
    );
    TEST_ASSERT_EQUAL_HEX8(0x3A, request[0]);
    TEST_ASSERT_EQUAL_HEX8(0x00, request[1]);
    TEST_ASSERT_EQUAL_HEX8(0x3A, request[2]);

    TEST_ASSERT_TRUE(
        KellyFrameCodec::buildRequest(0x3B, request)
    );
    TEST_ASSERT_EQUAL_HEX8(0x3B, request[0]);
    TEST_ASSERT_EQUAL_HEX8(0x00, request[1]);
    TEST_ASSERT_EQUAL_HEX8(0x3B, request[2]);
}

// TEST 2: Reject undocumented command 0x3C
void test_unsupported_command()
{
    uint8_t request[3] = {};

    TEST_ASSERT_FALSE(
        KellyFrameCodec::buildRequest(0x3C, request)
    );
}

// TEST 3: Correct response
void test_valid_response()
{
    uint8_t data[16] = {};
    uint8_t packet[19] = {};

    data[0] = 0x12;
    data[1] = 0x34;
    data[2] = 0x09;
    data[3] = 0xC4;

    TEST_ASSERT_TRUE(
        KellyFrameCodec::buildTestResponse(
            0x3B, data, packet
        )
    );

    TEST_ASSERT_TRUE(
        KellyFrameCodec::validateResponse(
            packet, 19, 0x3B
        )
    );

    uint8_t extracted[16] = {};

    TEST_ASSERT_TRUE(
        KellyFrameCodec::extractPayload(
            packet, 19, 0x3B, extracted
        )
    );

    TEST_ASSERT_EQUAL_UINT8_ARRAY(
        data, extracted, 16
    );
}

// TEST 4: Invalid checksum
void test_invalid_checksum()
{
    uint8_t data[16] = {};
    uint8_t packet[19] = {};

    KellyFrameCodec::buildTestResponse(
        0x3B, data, packet
    );

    packet[5] ^= 0x01;

    TEST_ASSERT_FALSE(
        KellyFrameCodec::validateResponse(
            packet, 19, 0x3B
        )
    );
}

// TEST 5: Wrong command echo
void test_wrong_command()
{
    uint8_t data[16] = {};
    uint8_t packet[19] = {};

    KellyFrameCodec::buildTestResponse(
        0x3A, data, packet
    );

    TEST_ASSERT_FALSE(
        KellyFrameCodec::validateResponse(
            packet, 19, 0x3B
        )
    );
}

// TEST 6: Truncated response
void test_incomplete_response()
{
    uint8_t data[16] = {};
    uint8_t packet[19] = {};

    KellyFrameCodec::buildTestResponse(
        0x3A, data, packet
    );

    TEST_ASSERT_FALSE(
        KellyFrameCodec::validateResponse(
            packet, 10, 0x3A
        )
    );
}

// TEST 7: Wrong length field
void test_wrong_length_field()
{
    uint8_t data[16] = {};
    uint8_t packet[19] = {};

    KellyFrameCodec::buildTestResponse(
        0x3A, data, packet
    );

    packet[1] = 0x0F;
    packet[18] =
        KellyFrameCodec::checksum(packet, 18);

    TEST_ASSERT_FALSE(
        KellyFrameCodec::validateResponse(
            packet, 19, 0x3A
        )
    );
}

// TEST 8: Null pointers
void test_null_inputs()
{
    uint8_t output[19] = {};

    TEST_ASSERT_FALSE(
        KellyFrameCodec::buildRequest(
            0x3B, nullptr
        )
    );

    TEST_ASSERT_FALSE(
        KellyFrameCodec::validateResponse(
            nullptr, 19, 0x3B
        )
    );

    TEST_ASSERT_FALSE(
        KellyFrameCodec::buildTestResponse(
            0x3B, nullptr, output
        )
    );
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_read_commands);
    RUN_TEST(test_unsupported_command);
    RUN_TEST(test_valid_response);
    RUN_TEST(test_invalid_checksum);
    RUN_TEST(test_wrong_command);
    RUN_TEST(test_incomplete_response);
    RUN_TEST(test_wrong_length_field);
    RUN_TEST(test_null_inputs);

    return UNITY_END();
}
