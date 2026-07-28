#pragma once

#include <array>
#include <cstdint>

struct RawPacket
{
    std::array<uint8_t, 22> data{};
};