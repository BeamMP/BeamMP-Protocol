#include "ModRequest.h"

size_t bmp::ModRequest::deserialize_from(std::span<const uint8_t> span) {
    return hash.deserialize_from(span);
}

size_t bmp::ModRequest::serialize_to(std::span<uint8_t> span) {
    return hash.serialize_to(span);
}
