#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>

namespace bmp {

struct ImplementationInfo {
    // at most 254 bytes
    std::string value;
    
    size_t deserialize_from(std::span<uint8_t> span);
    size_t serialize_to(std::span<uint8_t> span);

    auto operator<=>(const ImplementationInfo&) const = default;
};

}
