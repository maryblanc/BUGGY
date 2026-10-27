
#pragma once

#include <stddef.h>
#include <stdint.h>
#include "protocol/KellyFrameCodec.h"

class KellyFrameReceiver
{
public:
    explicit KellyFrameReceiver(uint8_t expectedCommand);

    // Clears a partial response.
    void reset();

    // Returns true when a complete valid frame is found.
    bool push(uint8_t byte);

    // Copies the payload from the last valid frame.
    bool getPayload(uint8_t out[16]) const;

private:
    uint8_t expectedCommand_;
    uint8_t buffer_[KellyFrameCodec::RESPONSE_SIZE] = {};
    size_t count_ = 0;
    bool ready_ = false;

    void discardFirst();
};
