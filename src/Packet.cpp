#include "Packet.h"
#include "Compression.h"

#include <algorithm>
#include <doctest/doctest.h>

std::vector<uint8_t> bmp::Packet::get_readable_data() const {
    if ((flags & bmp::Flags::ZstdCompressed) != 0) {
        return bmp::zstd_decompress(raw_data);
    } else {
        return raw_data;
    }
}

bmp::Header bmp::Packet::finalize() {
    // the user can force zstd compression on before setting data to force compression,
    // otherwise the threshold is used.
    if ((flags & bmp::Flags::ZstdCompressed) != 0
        || raw_data.size() > bmp::COMPRESSION_THRESHOLD) {
        flags = bmp::Flags(flags | bmp::Flags::ZstdCompressed);
        raw_data = bmp::zstd_compress(raw_data);
    }
    return {
        .purpose = purpose,
        .flags = flags,
        .rsv = 0,
        .size = static_cast<uint32_t>(raw_data.size()),
    };
}

TEST_CASE("Packet finalize") {
    bmp::Packet packet {
        .purpose = bmp::Purpose::Invalid,
    };
    SUBCASE("No compression, under threshold") {
        packet.raw_data = std::vector<uint8_t>(bmp::COMPRESSION_THRESHOLD - 1, 5);
        (void)packet.finalize();
        // not compressed, still the same
        CHECK(std::all_of(packet.raw_data.begin(), packet.raw_data.end(), [](uint8_t value) { return value == 5; }));
        ;
        // no compression flag
        CHECK_EQ(packet.flags & bmp::Flags::ZstdCompressed, 0);
    }
    SUBCASE("Compression via threshold") {
        packet.raw_data = std::vector<uint8_t>(bmp::COMPRESSION_THRESHOLD + 1, 5);
        (void)packet.finalize();
        // compressed, not the exact same
        CHECK(!std::all_of(packet.raw_data.begin(), packet.raw_data.end(), [](uint8_t value) { return value == 5; }));
        // decompressable
        CHECK_NOTHROW(bmp::zstd_decompress(packet.raw_data));
        // compression flag set
        CHECK_NE(packet.flags & bmp::Flags::ZstdCompressed, 0);
    }
    SUBCASE("Compression flag") {
        packet.raw_data = std::vector<uint8_t>(bmp::COMPRESSION_THRESHOLD - 1, 5);
        packet.flags = bmp::Flags(packet.flags | bmp::Flags::ZstdCompressed);
        (void)packet.finalize();
        // compressed, not the exact same
        CHECK(!std::all_of(packet.raw_data.begin(), packet.raw_data.end(), [](uint8_t value) { return value == 5; }));
        // decompressable
        CHECK_NOTHROW(bmp::zstd_decompress(packet.raw_data));
        // compression flag set
        CHECK_NE(packet.flags & bmp::Flags::ZstdCompressed, 0);
    }
}
