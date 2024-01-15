#pragma once

#include "Hash.h"
#include <cstdint>
#include <span>

namespace bmp {

struct ModRequest {
    Hash hash;

    size_t deserialize_from(std::span<const uint8_t> span);
    size_t serialize_to(std::span<uint8_t> span);

    auto operator<=>(const ModRequest&) const = default;
};

}