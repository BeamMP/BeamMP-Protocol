#pragma once

#include "Version.h"

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>

namespace bmp {

struct ProtocolVersion {
    Version version;

    size_t deserialize_from(std::span<const uint8_t> span);
    size_t serialize_to(std::span<uint8_t> span);

    auto operator<=>(const ProtocolVersion&) const = default;
};

}