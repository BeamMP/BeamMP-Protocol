#include "Transport.h"

#include <doctest/doctest.h>
#include <limits>
#include <vector>

#include "Util.h"

size_t bmp::Header::deserialize_from(std::span<uint8_t> span) {
    size_t offset = 0;
    uint16_t raw_purpose;
    offset += deserialize(raw_purpose, span.subspan(offset));
    purpose = Purpose(raw_purpose);
    uint8_t raw_flags;
    offset += deserialize(raw_flags, span.subspan(offset));
    flags = Flags(raw_flags);
    offset += deserialize(rsv, span.subspan(offset));
    offset += deserialize(size, span.subspan(offset));
    return offset;
}

size_t bmp::Header::serialize_to(std::span<uint8_t> span) {
    size_t offset = 0;
    offset += serialize(uint16_t(purpose), span.subspan(offset));
    offset += serialize(uint8_t(flags), span.subspan(offset));
    offset += serialize(rsv, span.subspan(offset));
    offset += serialize(size, span.subspan(offset));
    return offset;
}

TEST_CASE("Header serialization normal") {
    bmp::Header original {
        .purpose = bmp::Purpose::ApplyElectrics,
        .flags = bmp::Flags::ZstdCompressed,
        .rsv = 12,
        .size = std::numeric_limits<uint32_t>::max() / 2,
    };
    SUBCASE("Too small") {
        // too small by one
        std::vector<uint8_t> vec(7);
        CHECK_THROWS(original.serialize_to(vec));
    }
    SUBCASE("Normal") {
        std::vector<uint8_t> vec(8);
        CHECK_NOTHROW(original.serialize_to(vec));
        CHECK_EQ(original.serialize_to(vec), 8);
        bmp::Header new_hdr;
        CHECK_NOTHROW(new_hdr.deserialize_from(vec));
        CHECK_EQ(new_hdr, original);
        CHECK_EQ(new_hdr.deserialize_from(vec), 8);
    }
}
