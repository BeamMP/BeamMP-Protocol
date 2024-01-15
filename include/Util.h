#pragma once

#include <bit>
#include <concepts>
#include <cstdint>
#include <span>
#include <stdexcept>

namespace bmp {

namespace detail {
    template <size_t N>
    constexpr void byteswap_array(uint8_t (&bytes)[N]) {
        for (uint8_t *p = bytes, *end = bytes + N - 1; p < end; ++p, --end) {
            uint8_t tmp = *p;
            *p = *end;
            *end = tmp;
        }
    }
}

/// Performs an endianness byteswap on any integral type
template <std::integral T>
constexpr T byteswap(T value) {
    detail::byteswap_array(*reinterpret_cast<uint8_t(*)[sizeof(value)]>(&value));
    return value;
}

/// Deserializes the given datatype from a byte span.
/// Throws if there is not enough space to read the type.
template <typename T>
constexpr size_t deserialize(T& value, std::span<const uint8_t> span);

/// Deserializes an integer from a byte span.
/// Throws if there is not enough space to read the type.
template <std::integral T>
constexpr size_t deserialize(T& value, std::span<const uint8_t> span) {
    if (span.size_bytes() < sizeof(T)) {
        throw std::invalid_argument(std::string("span too small to deserialize type of size: ") + std::to_string(sizeof(T)));
    }
    // read to native integer
    value = *reinterpret_cast<const T*>(span.data());
    // swap endianness if needed
    if constexpr (std::endian::native == std::endian::big) {
        value = byteswap(value);
    }
    return sizeof(T);
}

/// Serializes the given datatype into the byte span and returns the number of bytes written.
/// Throws if there is not enough space to write the type.
template <typename T>
constexpr size_t serialize(const T& value, std::span<uint8_t> span);

/// Serializes the given datatype into the byte span.
/// Throws if there is not enough space to write the type.
template <std::integral T>
constexpr size_t serialize(const T& value, std::span<uint8_t> span) {
    if (span.size_bytes() < sizeof(T)) {
        throw std::invalid_argument(std::string("span too small to deserialize type of size: ") + std::to_string(sizeof(T)));
    }
    // swap endianness if needed
    if constexpr (std::endian::native == std::endian::big) {
        T real_value = byteswap(value);
        *reinterpret_cast<T*>(span.data()) = real_value;
    } else {
        // read to native integer
        *reinterpret_cast<T*>(span.data()) = value;
    }
    return sizeof(T);
}

}
