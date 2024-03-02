#include "ClientTransport.h"

#include "Util.h"

size_t bmp::ClientHeader::deserialize_from(std::span<const uint8_t> span) {
    size_t offset = 0;
    uint8_t raw_flags;
    offset += deserialize(raw_flags, span.subspan(offset));
    flags = ClientFlags(raw_flags);
    uint16_t raw_purpose;
    offset += deserialize(raw_purpose, span.subspan(offset));
    purpose = ClientPurpose(raw_purpose);
    offset += deserialize(pid, span.subspan(offset));
    offset += deserialize(vid, span.subspan(offset));
    offset += deserialize(data_size, span.subspan(offset));
    return offset;
}

size_t bmp::ClientHeader::serialize_to(std::span<uint8_t> span) {
    size_t offset = 0;
    offset += serialize(uint8_t(flags), span.subspan(offset));
    offset += serialize(uint16_t(purpose), span.subspan(offset));
    offset += serialize(pid, span.subspan(offset));
    offset += serialize(vid, span.subspan(offset));
    offset += serialize(data_size, span.subspan(offset));
    return offset;
}
