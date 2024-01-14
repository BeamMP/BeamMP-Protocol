#include "Hash.h"

#include <algorithm>
#include <doctest/doctest.h>
#include <vector>

std::string bmp::to_string(const bmp::HashAlgorithm& algo) {
    switch (algo) {
    case bmp::HashAlgorithm::Sha256:
        return "sha256";
    default:
        throw std::invalid_argument("The given algorithm is not valid and cannot be stringified");
    }
}
size_t bmp::hash_size(const bmp::HashAlgorithm& algo) {
    switch (algo) {
    case bmp::HashAlgorithm::Sha256:
        return 64;
    default:
        throw std::invalid_argument("The given algorithm is not valid, so the hash size cannot be determined");
    }
}

size_t bmp::Hash::deserialize_from(std::span<uint8_t> span) {
    auto loc = std::find(span.begin(), span.end(), ':');
    if (loc == span.end()) {
        throw std::invalid_argument("Hash doesn't contain ':'");
    }
    std::string algo(span.begin(), loc);
    if (algo == to_string(HashAlgorithm::Sha256)) {
        algorithm = HashAlgorithm::Sha256;
    } else {
        throw std::invalid_argument("The given algorithm is not valid: '" + algo + "'");
    }
    auto hash_size = ::bmp::hash_size(algorithm);
    if (span.size() - algo.size() - 1 < hash_size) {
        throw std::invalid_argument("The given span is too small to hold " + std::to_string(hash_size) + " bytes of hash");
    }
    hash = std::string(loc + 1, loc + 1 + hash_size);
    return loc + 1 + hash_size - span.begin();
}

size_t bmp::Hash::serialize_to(std::span<uint8_t> span) {
    auto hash_size = ::bmp::hash_size(algorithm);
    auto str_algo = bmp::to_string(algorithm);
    if (span.size() < str_algo.size() + 1 + hash_size) {
        throw std::invalid_argument("Span too small to hold Hash");
    }
    if (hash.size() != hash_size) {
        throw std::runtime_error("Size of the contained hash (" + std::to_string(hash.size()) + " is not expected size " + std::to_string(hash_size));
    }
    std::copy(str_algo.begin(), str_algo.end(), span.begin());
    span[str_algo.size()] = ':';
    std::copy(hash.begin(), hash.end(), span.begin() + str_algo.size() + 1);
    return str_algo.size() + 1 + hash_size;
}

TEST_CASE("Hash serialization") {
    bmp::Hash original {
        .algorithm = bmp::HashAlgorithm::Sha256,
        .hash = "048d892fe729e2a82e7cac19c96a107a0882d4a60a813ba31c22e64c1d8428f3",
    };
    SUBCASE("Normal") {
        std::vector<uint8_t> vec(100);
        CHECK_NOTHROW(original.serialize_to(vec));
        bmp::Hash hash;
        std::cout << std::string(vec.begin(), vec.end()) << std::endl;
        CHECK_NOTHROW(hash.deserialize_from(vec));
        CHECK_EQ(hash, original);
    }
}