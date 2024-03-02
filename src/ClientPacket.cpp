#include "ClientPacket.h"

std::vector<uint8_t> bmp::ClientPacket::get_readable_data() const {
    return raw_data;
}

bmp::ClientHeader bmp::ClientPacket::finalize() {
    return {
        .flags = flags,
        .purpose = purpose,
        .pid = pid,
        .vid = vid,
        .data_size = static_cast<uint32_t>(raw_data.size()),
    };
}
