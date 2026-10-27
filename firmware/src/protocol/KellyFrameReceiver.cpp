
#include "protocol/KellyFrameReceiver.h"

KellyFrameReceiver::KellyFrameReceiver(uint8_t expectedCommand)
    : expectedCommand_(expectedCommand)
{
    reset();
}

void KellyFrameReceiver::reset()
{
    count_ = 0;
    ready_ = false;
}

void KellyFrameReceiver::discardFirst()
{
    if (count_ == 0)
    {
        return;
    }

    for (size_t i = 1; i < count_; ++i)
    {
        buffer_[i - 1] = buffer_[i];
    }

    --count_;
}

bool KellyFrameReceiver::push(uint8_t byte)
{
    // Preserve a completed response until reset().
    if (ready_)
    {
        return true;
    }

    if (count_ < KellyFrameCodec::RESPONSE_SIZE)
    {
        buffer_[count_++] = byte;
    }

    // Find a possible frame beginning.
    while (count_ > 0)
    {
        if (buffer_[0] != expectedCommand_)
        {
            discardFirst();
            continue;
        }

        if (count_ >= 2 &&
            buffer_[1] != KellyFrameCodec::DATA_SIZE)
        {
            discardFirst();
            continue;
        }

        if (count_ < KellyFrameCodec::RESPONSE_SIZE)
        {
            return false;
        }

        if (KellyFrameCodec::validateResponse(
                buffer_,
                KellyFrameCodec::RESPONSE_SIZE,
                expectedCommand_))
        {
            ready_ = true;
            return true;
        }

        // Invalid checksum: slide one byte and
        // continue looking for a valid frame.
        discardFirst();
    }

    return false;
}

bool KellyFrameReceiver::getPayload(uint8_t out[16]) const
{
    if (!ready_ || out == nullptr)
    {
        return false;
    }

    return KellyFrameCodec::extractPayload(
        buffer_,
        KellyFrameCodec::RESPONSE_SIZE,
        expectedCommand_,
        out
    );
}
