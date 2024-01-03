#include <gtest/gtest.h>
#include <gtest/gtest-matchers.h>
#include <gmock/gmock-more-matchers.h>

#include <string>

#include <hash/hash-tools.h>

using namespace ::testing;


TEST(Tools, ByteSwapUint8)
{
    uint8_t data[] = {0xd4, 0x1d, 0x8c, 0xd9, 0xde, 0xad, 0xbe, 0xef};
    uint8_t expected[] = {0xd4, 0x1d, 0x8c, 0xd9, 0xde, 0xad, 0xbe, 0xef};
    Hash::byteswap(data);
    for(int idx = 0 ; idx < 8 ; ++idx){
            EXPECT_THAT(data[idx], Eq(expected[idx]));
    }
}

TEST(Tools, ByteSwapUint16)
{
    uint8_t data[] = {0xd4, 0x1d, 0x8c, 0xd9, 0xde, 0xad, 0xbe, 0xef};
    uint8_t expected[] = {0x1d, 0xd4, 0xd9, 0x8c, 0xad, 0xde, 0xef, 0xbe};
    uint16_t input[4];
    uint16_t output[4];

    memcpy(input, data, 8);
    memcpy(output, expected, 8);
    for(int idx = 0 ; idx < 4 ; ++idx){
        Hash::byteswap(&input[idx]);
        EXPECT_THAT(input[idx], Eq(output[idx]));
    }
}

TEST(Tools, ByteSwapUint32)
{
    uint8_t data[] = {0xd4, 0x1d, 0x8c, 0xd9, 0xde, 0xad, 0xbe, 0xef};
    uint8_t expected[] = {0xd9, 0x8c, 0x1d, 0xd4, 0xef, 0xbe, 0xad, 0xde};
    uint32_t input[2];
    uint32_t output[2];

    memcpy(input, data, 8);
    memcpy(output, expected, 8);
    for(int idx = 0 ; idx < 2 ; ++idx){
        Hash::byteswap(&input[idx]);
        EXPECT_THAT(input[idx], Eq(output[idx]));
    }
}

TEST(Tools, ByteSwapUint64)
{
    uint8_t data[] = {0xd4, 0x1d, 0x8c, 0xd9, 0xde, 0xad, 0xbe, 0xef};
    uint8_t expected[] = {0xef, 0xbe, 0xad, 0xde, 0xd9, 0x8c, 0x1d, 0xd4};
    uint64_t input[1];
    uint64_t output[1];

    memcpy(input, data, 8);
    memcpy(output, expected, 8);
    for(int idx = 0 ; idx < 1 ; ++idx){
        Hash::byteswap(&input[idx]);
        EXPECT_THAT(input[idx], Eq(output[idx]));
    }
}

TEST(Tools, ByteSwapOnBigEndian)
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

TEST(Tools, HashToString)
{
    uint8_t input[] = {0xe4, 0xd9, 0x09, 0xc2, 0x90, 0xd0, 0xfb, 0x1c, 0xa0, 0x68, 0xff, 0xad, 0xdf, 0x22, 0xcb, 0xd0};
    std::string expected = "e4d909c290d0fb1ca068ffaddf22cbd0";
    EXPECT_THAT(Hash::to_string(input, 16), Eq(expected));
}

TEST(Tools, StringToHash)
{
    std::string input = "e4d909c290d0fb1ca068ffaddf22cbd0";
    std::vector<uint8_t> expected{0xe4, 0xd9, 0x09, 0xc2, 0x90, 0xd0, 0xfb, 0x1c, 0xa0, 0x68, 0xff, 0xad, 0xdf, 0x22, 0xcb, 0xd0};
    EXPECT_THAT(Hash::from_string(input), ContainerEq(expected));
}
