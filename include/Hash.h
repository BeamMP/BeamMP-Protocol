#pragma once

#include <cstdint>
#include <span>
#include <stdexcept>
#include <string>
namespace bmp {

enum HashAlgorithm {
    Sha256,
};

struct Hash {
    HashAlgorithm algorithm;
    std::string hash;
    size_t deserialize_from(std::span<uint8_t> span);
    size_t serialize_to(std::span<uint8_t> span);

    auto operator<=>(const Hash&) const = default;
};

std::string to_string(const bmp::HashAlgorithm& algo);
/// Returns the size of the hash in characters in hex (2 * size in bytes)
size_t hash_size(const bmp::HashAlgorithm& algo);

}
