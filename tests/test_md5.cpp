#include <gtest/gtest.h>
#include <gtest/gtest-matchers.h>
#include <gmock/gmock-more-matchers.h>

#include <string>

#include <hash/hash-tools.h>
#include <hash/md5.h>

using namespace ::testing;

TEST(Md5, ByteSwapUint8)
{
    uint8_t data[] = {0xd4, 0x1d, 0x8c, 0xd9};
    uint8_t expected[] = {0xd4, 0x1d, 0x8c, 0xd9};
    Hash::byteswap(data);
    for(int idx = 0 ; idx < 4 ; ++idx){
            EXPECT_THAT(data[idx], Eq(expected[idx]));
    }
}

TEST(Md5, ByteSwapOnBigEndian)
{
    uint8_t data[] = {0xd4, 0x1d, 0x8c, 0xd9};
    uint8_t converted[4];
    Hash::byteswap_to_le(converted, data);
    for(int idx = 0 ; idx < 4 ; ++idx)
    {
        if constexpr (std::endian::native == std::endian::big)
            EXPECT_THAT(data[idx], Eq(converted[3 - idx]));
        else
            EXPECT_THAT(data[idx], Eq(converted[idx]));
    }
}

TEST(Md5, EmptyString)
{
    uint8_t result[16];
    const char* input = "";
    Hash::md5_hasher::from_string(input, result);
    uint8_t expected[] = {0xd4, 0x1d, 0x8c, 0xd9, 0x8f, 0x00, 0xb2, 0x04, 0xe9, 0x80, 0x09, 0x98, 0xec, 0xf8, 0x42, 0x7e};
    for(int idx = 0 ; idx < 16 ; ++idx)
        EXPECT_THAT(result[idx], Eq(expected[idx]));
}

TEST(Md5, SimpleString)
{
    uint8_t result[16];
    const char* input = "Hello, World!";
    Hash::md5_hasher::from_string(input, result);
    uint8_t expected[] = {0x65, 0xa8, 0xe2, 0x7d, 0x88, 0x79, 0x28, 0x38, 0x31, 0xb6, 0x64, 0xbd, 0x8b, 0x7f, 0x0a, 0xd4};
    for(int idx = 0 ; idx < 16 ; ++idx)
        EXPECT_THAT(result[idx], Eq(expected[idx]));
}

TEST(Md5, QuickBrownFox)
{
    uint8_t result[16];
    const char* input = "The quick brown fox jumps over the lazy dog.";
    Hash::md5_hasher::from_string(input, result);
    uint8_t expected[] = {0xe4, 0xd9, 0x09, 0xc2, 0x90, 0xd0, 0xfb, 0x1c, 0xa0, 0x68, 0xff, 0xad, 0xdf, 0x22, 0xcb, 0xd0};
    for(int idx = 0 ; idx < 16 ; ++idx)
        EXPECT_THAT(result[idx], Eq(expected[idx]));
}
