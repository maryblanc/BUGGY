
#pragma once

#include <stddef.h>
#include <stdint.h>

enum class KellyLinkState : uint8_t
{
    NotStarted,
    OK,
    NoResponse,
    InvalidFrame,
    SendError
};

class KellyLinkStatus
{
public:
    void beginRequest()
    {
        state_ = KellyLinkState::NotStarted;
        receivedBytes_ = 0;
    }

    void recordReceivedByte()
    {
        ++receivedBytes_;
    }

    void markSuccess()
    {
        state_ = KellyLinkState::OK;
    }

    void markSendError()
    {
        state_ = KellyLinkState::SendError;
    }

    void markTimeout()
    {
        state_ = receivedBytes_ == 0
            ? KellyLinkState::NoResponse
            : KellyLinkState::InvalidFrame;
    }

    KellyLinkState state() const
    {
        return state_;
    }

    size_t receivedBytes() const
    {
        return receivedBytes_;
    }

    bool hasValidData() const
    {
        return state_ == KellyLinkState::OK;
    }

    const char* text() const
    {
        switch (state_)
        {
            case KellyLinkState::OK:
                return "OK";

            case KellyLinkState::NoResponse:
                return "NO_RESPONSE";

            case KellyLinkState::InvalidFrame:
                return "INVALID_FRAME";

            case KellyLinkState::SendError:
                return "SEND_ERROR";

            default:
                return "NOT_STARTED";
        }
    }

private:
    KellyLinkState state_ = KellyLinkState::NotStarted;
    size_t receivedBytes_ = 0;
};
