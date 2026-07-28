#include "models/TelemetryFrame.h"
#include "models/RawPacket.h"


class TelemetryParser
{
public:
    TelemetryFrame parse(const RawPacket& packet);

};