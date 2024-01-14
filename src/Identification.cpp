#include "Identification.h"
#include "Util.h"
#include <algorithm>
#include <doctest/doctest.h>
#include <stdexcept>
#include <vector>

size_t bmp::Version::deserialize_from(std::span<uint8_t> span) {
    size_t offset = 0;
    offset += deserialize(major, span.subspan(offset));
    offset += deserialize(minor, span.subspan(offset));
    offset += deserialize(patch, span.subspan(offset));
    return offset;
}

size_t bmp::Version::serialize_to(std::span<uint8_t> span) {
    size_t offset = 0;
    offset += serialize(major, span.subspan(offset));
    offset += serialize(minor, span.subspan(offset));
    offset += serialize(patch, span.subspan(offset));
    return offset;
}

TEST_CASE("Version serialization") {
    bmp::Version original {
        .major = 0x1234,
        .minor = 0x5624,
        .patch = 0x7531,
    };
    SUBCASE("Too small") {
        std::vector<uint8_t> vec(5);
        CHECK_THROWS(original.serialize_to(vec));
        CHECK_THROWS(original.deserialize_from(vec));
    }
    SUBCASE("Normal") {
        std::vector<uint8_t> vec(6);
        CHECK_NOTHROW(original.serialize_to(vec));
        bmp::Version new_ver;
        CHECK_NOTHROW(new_ver.deserialize_from(vec));
        CHECK_EQ(new_ver, original);
        CHECK_EQ(new_ver.deserialize_from(vec), 6);
        CHECK_EQ(new_ver.serialize_to(vec), 6);
    }
}

size_t bmp::ImplementationInfo::serialize_to(std::span<uint8_t> span) {
    if (value.size() > 254) {
        throw std::invalid_argument("Implementation info exceeds maximum size of 254 bytes");
    }
    if (span.size() < value.size() + 1) {
        throw std::invalid_argument("Given span doesn't have enough space for implementation info");
    }
    size_t offset = serialize(uint8_t(value.size()), span);
    std::copy(value.begin(), value.end(), span.begin() + offset);
    return offset + value.size();
}

size_t bmp::ImplementationInfo::deserialize_from(std::span<uint8_t> span) {
    size_t offset = 0;
    uint8_t size = 0;
    offset += deserialize(size, span);
    if (span.size() < size + offset) {
        throw std::invalid_argument("Span has too little space to hold implementation info");
    }
    value = std::string(span.begin() + offset, span.begin() + offset + size);
    return offset + size;
}

TEST_CASE("ImplementationInfo serialization") {
    bmp::ImplementationInfo original {
        .value = "Original BeamMP Server (tm) (R) (Cool) (Epic)!!!!",
    };
    SUBCASE("Too small") {
        std::vector<uint8_t> vec(15);
        CHECK_THROWS(original.serialize_to(vec));
        // one byte too little
        std::vector<uint8_t> vec2(original.value.size());
        CHECK_THROWS(original.serialize_to(vec2));
        // dynamic space
        std::vector<uint8_t> vec3(15);
        vec3[0] = 240;
        CHECK_THROWS(original.deserialize_from(vec3));
    }
    SUBCASE("Normal") {
        std::vector<uint8_t> vec(original.value.size() + 1);
        CHECK_NOTHROW(original.serialize_to(vec));
        bmp::ImplementationInfo new_impl;
        CHECK_NOTHROW(new_impl.deserialize_from(vec));
        CHECK_EQ(new_impl, original);
        CHECK_EQ(new_impl.deserialize_from(vec), original.value.size() + 1);
        CHECK_EQ(new_impl.serialize_to(vec), original.value.size() + 1);
    }
}
