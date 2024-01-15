#include "ProtocolVersion.h"

size_t bmp::ProtocolVersion::deserialize_from(std::span<const uint8_t> span) {
    size_t offset = 0;
    offset += version.deserialize_from(span.subspan(offset));
    return offset;
}

size_t bmp::ProtocolVersion::serialize_to(std::span<uint8_t> span) {
    size_t offset = 0;
    offset += version.serialize_to(span.subspan(offset));
    return offset;
}