#pragma once

#include "ImplementationInfo.h"
#include "Version.h"

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>

namespace bmp {

struct ServerInfo {
    Version program_version;
    ImplementationInfo implementation;

    size_t deserialize_from(std::span<uint8_t> span);
    size_t serialize_to(std::span<uint8_t> span);

    auto operator<=>(const ServerInfo&) const = default;
};

}
