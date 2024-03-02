#pragma once

#include "Transport.h"
#include <vector>

namespace bmp {

struct Packet {
    Purpose purpose {};
    Flags flags {};

    /// Returns data with consideration to flags.
    std::vector<uint8_t> get_readable_data() const;

    /// Sets flags (e.g. compression flag) if the data is above some threshold,
    /// and compresses the data.
    /// Returns the header needed to send this packet.
    [[nodiscard]] Header finalize();

    /// Raw (potentially compressed) data -- do not read directly to deserialize from.
    std::vector<uint8_t> raw_data;
};

}
