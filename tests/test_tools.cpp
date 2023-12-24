#include <gtest/gtest.h>
#include <gtest/gtest-matchers.h>
#include <gmock/gmock-more-matchers.h>

#include <string>

#include <hash/hash-tools.h>

namespace fs = std::filesystem;
using namespace ::testing;

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
    EXPECT_THAT(Hash::from_string(input), Eq(expected));
}
