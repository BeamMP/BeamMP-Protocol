#pragma once

#include "Version.h"
#include "ImplementationInfo.h"
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>

namespace bmp {


struct ClientInfo {
    Version program_version;
    Version game_version;
    Version mod_version;
    ImplementationInfo implementation;
    
    size_t deserialize_from(std::span<uint8_t> span);
    size_t serialize_to(std::span<uint8_t> span);
    
    auto operator<=>(const ClientInfo&) const = default;
};

}
