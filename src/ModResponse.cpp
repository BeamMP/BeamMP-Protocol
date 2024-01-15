#include "ModResponse.h"

size_t bmp::ModResponse::deserialize_from(std::span<const uint8_t> span) {
    auto offset = hash.deserialize_from(span);
    return offset;
}

size_t bmp::ModResponse::serialize_to(std::span<uint8_t> span) {
    auto offset = hash.serialize_to(span);
    return offset;
}
