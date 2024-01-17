#include "Compression.h"

#include <algorithm>
#include <fmt/format.h>
#include <memory>

struct ZstdCDeleter {
    void operator()(ZSTD_CCtx* cctx) {
        ZSTD_freeCCtx(cctx);
    }
};

struct ZstdDDeleter {
    void operator()(ZSTD_DCtx* dctx) {
        ZSTD_freeDCtx(dctx);
    }
};

ZSTD_CCtx* bmp::detail::get_cctx() {
    static thread_local std::unique_ptr<ZSTD_CCtx, ZstdCDeleter> cctx { nullptr };
    if (cctx == nullptr) {
        cctx = decltype(cctx)(ZSTD_createCCtx());
    }
    return cctx.get();
}

ZSTD_DCtx* bmp::detail::get_dctx() {
    static thread_local std::unique_ptr<ZSTD_DCtx, ZstdDDeleter> dctx { nullptr };
    if (dctx == nullptr) {
        dctx = decltype(dctx)(ZSTD_createDCtx());
    }
    return dctx.get();
}

std::vector<uint8_t> bmp::zstd_decompress(std::span<const uint8_t> in) {
    auto contentsize = ZSTD_getFrameContentSize(in.data(), in.size());
    if (contentsize == ZSTD_CONTENTSIZE_ERROR) {
        throw std::runtime_error("Unknown error while decompressing packet");
    } else if (contentsize == ZSTD_CONTENTSIZE_UNKNOWN) {
        throw std::runtime_error("Size of decompressed load unknown, this is not expected by the implementation");
    }
    std::vector<uint8_t> out(contentsize);
    auto res = ZSTD_decompressDCtx(detail::get_dctx(), out.data(), out.size(), in.data(), in.size());
    if (ZSTD_isError(res)) {
        throw std::runtime_error(std::string("Zstd decompression failed: ") + ZSTD_getErrorName(res));
    }
    out.resize(res);
    return out;
}

std::vector<uint8_t> bmp::zstd_compress(std::span<const uint8_t> in) {
    auto worst_case_size = ZSTD_compressBound(in.size());
    std::vector<uint8_t> out(worst_case_size);
    auto res = ZSTD_compress2(detail::get_cctx(), out.data(), out.size(), in.data(), in.size());
    if (ZSTD_isError(res)) {
        throw std::runtime_error(std::string("Zstd compression failed: ") + ZSTD_getErrorName(res));
    }
    out.resize(res);
    return out;
}

#include <doctest/doctest.h>

TEST_CASE("zstd_compress and zstd_decompress") {
    // generate some predictably compressable data
    std::vector<uint8_t> vec {};
    vec.resize(4096 * 2);
    for (size_t i = 0; i < 4096; ++i) {
        vec[i] = i * 3141;
    }
    for (size_t i = 4096; i < 4096 * 2; ++i) {
        vec[i] = vec[i - 4096];
    }

    SUBCASE("Normal") {
        auto compressed = bmp::zstd_compress(vec);
        CHECK_LT(compressed.size(), vec.size());
        auto decompressed = bmp::zstd_decompress(compressed);
        CHECK(std::equal(vec.begin(), vec.end(), decompressed.begin(), decompressed.end()));
    }
    SUBCASE("Example packet") {
        std::string packet = R"({"_id":"65a7d0dddac1c4c1f6e9d001","guid":"d93fcd81-bf3a-459c-8263-d60b10bd9ae0","isActive":true,"pos":[5.2892,7.0291,1.8677],"rot":[4.7116,9.022,0.8564],"bruh":[0.9724,0.785,0.9655],"stuff":[5.3605,1.3198,0.9216]})";
        std::vector<uint8_t> str(packet.begin(), packet.end());
        auto compressed = bmp::zstd_compress(str);
        CHECK_LT(compressed.size(), str.size());
        auto decompressed = bmp::zstd_decompress(compressed);
        CHECK(std::equal(str.begin(), str.end(), decompressed.begin(), decompressed.end()));
    }
    SUBCASE("Not compressed") {
        CHECK_THROWS(bmp::zstd_decompress(vec));
    }
    SUBCASE("Empty") {
        CHECK_NOTHROW(bmp::zstd_compress({}));
        CHECK_THROWS(bmp::zstd_decompress({}));
    }
}
