#include <Utils.hpp>

#include <tuple>
#include <type_traits>

#include <gtest/gtest.h>

namespace roen
{

TEST(HashStringTest, ShouldHash)
{
    const std::string stringToBeHashed{"abcd"};

    EXPECT_EQ(typeid(std::uint64_t), typeid(hashString(stringToBeHashed)));
}

TEST(HashStringTest, HashShouldBeTheSameForTheSameString)
{
    const std::string stringToBeHashed{"abcd"};

    EXPECT_EQ(hashString(stringToBeHashed), hashString(stringToBeHashed));
}

TEST(HashStringTest, HashShouldBeTheDifferentForDifferentString)
{
    const std::string firstStringToBeHashed{"abcd"};
    const std::string secondStringToBeHashed{"abce"};

    EXPECT_NE(hashString(firstStringToBeHashed), hashString(secondStringToBeHashed));
}

class PowerTest
    : public testing::TestWithParam<std::tuple<std::int32_t, std::uint32_t, std::int32_t>>
{
};

INSTANTIATE_TEST_SUITE_P(PowerTesting, PowerTest,
                         ::testing::Values(std::make_tuple(2, 2, 4), std::make_tuple(2, 3, 8),
                                           std::make_tuple(3, 2, 9), std::make_tuple(3, 3, 27),
                                           std::make_tuple(-5, 2, 25), std::make_tuple(-5, 3, -125),
                                           std::make_tuple(31, 2, 961),
                                           std::make_tuple(31, 5, 28629151),
                                           std::make_tuple(31, 6, 887503681)));

TEST_P(PowerTest, ShouldCorrectlyCalculatePower)
{
    const auto& [base, exponent, expectedValue] = GetParam();

    EXPECT_EQ(pow(base, exponent), expectedValue);
}

}  // namespace roen
