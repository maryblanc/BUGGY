#include "RawPacket.h"

class KellyProtocol
{
    public:
        bool begin();

        bool ReadPacker(RawPacket& packet);
};