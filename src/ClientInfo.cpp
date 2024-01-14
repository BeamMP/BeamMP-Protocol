#include "ClientInfo.h"

#include <doctest/doctest.h>
#include <vector>

size_t bmp::ClientInfo::deserialize_from(std::span<uint8_t> span) {
    size_t offset = 0;
    offset += program_version.deserialize_from(span.subspan(offset));
    offset += game_version.deserialize_from(span.subspan(offset));
    offset += mod_version.deserialize_from(span.subspan(offset));
    offset += implementation.deserialize_from(span.subspan(offset));
    return offset;
}

size_t bmp::ClientInfo::serialize_to(std::span<uint8_t> span) {
    size_t offset = 0;
    offset += program_version.serialize_to(span.subspan(offset));
    offset += game_version.serialize_to(span.subspan(offset));
    offset += mod_version.serialize_to(span.subspan(offset));
    offset += implementation.serialize_to(span.subspan(offset));
    return offset;
}

TEST_CASE("ClientInfo serialization") {
    bmp::ClientInfo original {
        .program_version = {
            .major = 0x4352,
            .minor = 0x1f34,
            .patch = 0x9013,
        },
        .game_version = {
            .major = 0x2356,
            .minor = 0x54a3,
            .patch = 0x3743,
        },
        .mod_version = {
            .major = 0x251f,
            .minor = 0xab24,
            .patch = 0x45f5,
        },
        .implementation = {
            .value = "Hello, epic world!",
        },
    };
    SUBCASE("Normal") {
        std::vector<uint8_t> vec(1024);
        CHECK_NOTHROW(original.serialize_to(vec));
        bmp::ClientInfo new_info;
        CHECK_NOTHROW(new_info.deserialize_from(vec));
        CHECK_EQ(new_info, original);
    }
}
