#include "ServerInfo.h"

#include <doctest/doctest.h>
#include <vector>

size_t bmp::ServerInfo::deserialize_from(std::span<const uint8_t> span) {
    size_t offset = 0;
    offset += program_version.deserialize_from(span.subspan(offset));
    offset += implementation.deserialize_from(span.subspan(offset));
    return offset;
}

size_t bmp::ServerInfo::serialize_to(std::span<uint8_t> span) {
    size_t offset = 0;
    offset += program_version.serialize_to(span.subspan(offset));
    offset += implementation.serialize_to(span.subspan(offset));
    return offset;
}

TEST_CASE("ServerInfo serialization") {
    bmp::ServerInfo original {
        .program_version = {
            .major = 0x4352,
            .minor = 0x1f34,
            .patch = 0x9013,
        },
        .implementation = {
            .value = "Hello, epic world!",
        },
    };
    SUBCASE("Normal") {
        std::vector<uint8_t> vec(1024);
        CHECK_NOTHROW(original.serialize_to(vec));
        bmp::ServerInfo new_info;
        CHECK_NOTHROW(new_info.deserialize_from(vec));
        CHECK_EQ(new_info, original);
    }
}
