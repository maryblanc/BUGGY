
#include <unity.h>

#include "protocol/KellyLinkStatus.h"
#include <cstring>

void setUp() {}
void tearDown() {}

void test_initial_state()
{
    KellyLinkStatus status;

    TEST_ASSERT_EQUAL(
        static_cast<int>(KellyLinkState::NotStarted),
        static_cast<int>(status.state())
    );

    TEST_ASSERT_FALSE(status.hasValidData());
    TEST_ASSERT_EQUAL_UINT32(0, status.receivedBytes());
}

void test_no_response()
{
    KellyLinkStatus status;

    status.beginRequest();
    status.markTimeout();

    TEST_ASSERT_EQUAL_STRING(
        "NO_RESPONSE", status.text()
    );

    TEST_ASSERT_FALSE(status.hasValidData());
}

void test_invalid_frame()
{
    KellyLinkStatus status;

    status.beginRequest();
    status.recordReceivedByte();
    status.recordReceivedByte();
    status.markTimeout();

    TEST_ASSERT_EQUAL_STRING(
        "INVALID_FRAME", status.text()
    );

    TEST_ASSERT_EQUAL_UINT32(2, status.receivedBytes());
    TEST_ASSERT_FALSE(status.hasValidData());
}

void test_success()
{
    KellyLinkStatus status;

    status.beginRequest();
    status.recordReceivedByte();
    status.markSuccess();

    TEST_ASSERT_EQUAL_STRING("OK", status.text());
    TEST_ASSERT_TRUE(status.hasValidData());
}

void test_send_error()
{
    KellyLinkStatus status;

    status.beginRequest();
    status.markSendError();

    TEST_ASSERT_EQUAL_STRING(
        "SEND_ERROR", status.text()
    );
}

void test_reset_after_success()
{
    KellyLinkStatus status;

    status.markSuccess();
    status.beginRequest();

    TEST_ASSERT_FALSE(status.hasValidData());
    TEST_ASSERT_EQUAL_STRING(
        "NOT_STARTED", status.text()
    );

    TEST_ASSERT_EQUAL_UINT32(0, status.receivedBytes());
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_initial_state);
    RUN_TEST(test_no_response);
    RUN_TEST(test_invalid_frame);
    RUN_TEST(test_success);
    RUN_TEST(test_send_error);
    RUN_TEST(test_reset_after_success);

    return UNITY_END();
}
