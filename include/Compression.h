#pragma once

#include <cstdint>
#include <span>
#include <vector>
#include <zstd.h>

namespace bmp {

/// On the facebook/zstd repo, discussions regularly mention that compression below <1KB
/// without dictionary compression results in poor compression results. We go some number
/// below this arbitrary number, because we consider that we have many tiny packets
/// which add up over time, so even a 10 byte improvement is significant (over time).
/// Packets which we *know* we send a lot of and which we *know* compress well should
/// of course override this threshold.
constexpr size_t COMPRESSION_THRESHOLD = 256;

namespace detail {

    /// Lazily creates and manages the memory of a zstd compression context.
    /// Do not free the pointer this returns.
    ZSTD_CCtx* get_cctx();
    /// Lazily creates and manages the memory of a zstd decompression context.
    /// Do not free the pointer this returns.
    ZSTD_DCtx* get_dctx();
}

std::vector<uint8_t> zstd_decompress(std::span<const uint8_t> in);
std::vector<uint8_t> zstd_compress(std::span<const uint8_t> in);

}
