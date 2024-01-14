#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>

namespace bmp {
struct Version {
    uint16_t major;
    uint16_t minor;
    uint16_t patch;

    size_t deserialize_from(std::span<uint8_t> span);
    size_t serialize_to(std::span<uint8_t> span);

    auto operator<=>(const Version&) const = default;
};
}
