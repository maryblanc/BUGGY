
#pragma once

#include <stdint.h>

#include "controllers/KellyParser.h"
#include "models/TelemetryFrame.h"

class KellyTelemetryAssembler
{
public:
    static bool assemble(
        bool monitor1Ok,
        const uint8_t* monitor1,
        bool monitor2Ok,
        const uint8_t* monitor2,
        KellyParser& parser,
        TelemetryFrame& output
    )
    {
        // Always invalidate previous data first.
        output = TelemetryFrame{};

        if (!monitor1Ok || !monitor2Ok ||
            monitor1 == nullptr || monitor2 == nullptr)
        {
            return false;
        }

        output = parser.parse(monitor1, monitor2);
        return true;
    }
};
