#pragma once
#include <cstdint>
#include <cstddef>

struct UARTPacket
{
    static constexpr std::size_t MONITOR_SIZE = 16;
    uint8_t monitor1[MONITOR_SIZE];
    uint8_t monitor2[MONITOR_SIZE];
};