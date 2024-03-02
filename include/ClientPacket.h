#pragma once

#include "ClientTransport.h"
#include <vector>

namespace bmp {

struct ClientPacket {
    ClientFlags flags {};
    ClientPurpose purpose {};
    uint32_t pid { ClientHeader::INVALID_PID };
    uint16_t vid { ClientHeader::INVALID_VID };

    /// Returns data with consideration to flags.
    std::vector<uint8_t> get_readable_data() const;

    /// Sets flags (e.g. msgpack flag) if the data is above some threshold,
    /// and encodes the data.
    /// Returns the header needed to send this packet.
    [[nodiscard]] ClientHeader finalize();

    /// Raw (potentially encoded) data -- do not read directly to deserialize from.
    std::vector<uint8_t> raw_data;
};

}
