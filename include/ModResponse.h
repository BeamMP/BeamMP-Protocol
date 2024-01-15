#pragma once

#include "Hash.h"
#include <cstdint>
#include <span>
#include <filesystem>

namespace bmp {

/// MUST send the mod itself separately, after serialize, because there is no
/// efficient, implementation-agnostic way of sending a large file via tcp without
/// involving socket abstractions.
struct ModResponse {
    Hash hash;

    size_t deserialize_from(std::span<const uint8_t> span);
    size_t serialize_to(std::span<uint8_t> span);

    auto operator<=>(const ModResponse&) const = default;
};

}