#include "Version.h"

#include "Util.h"
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
