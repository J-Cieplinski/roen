#include <data_structure/Vector2.hpp>

#include <gtest/gtest.h>

namespace roen::data_structure
{

class Vector2Tests : public testing::Test
{
};

TEST_F(Vector2Tests, shouldCorrectlyConstructVector)
{
    constexpr auto expectedX = 2.f;
    constexpr auto expectedY = 24.5f;

    Vector2f v{expectedX, expectedY};

    EXPECT_EQ(v.x, expectedX);
    EXPECT_EQ(v.y, expectedY);
}

TEST_F(Vector2Tests, shouldProperlyCheckIfVectorsAreEqual)
{
    Vector2 v1{5.f, 5.f};
    Vector2 v2 = v1;
    EXPECT_EQ(v1, v2);
}

TEST_F(Vector2Tests, shouldProperlyCheckIfVectorsAreNotEqual)
{
    Vector2 v1{5.f, 5.f};
    Vector2 v2{10.f, 10.f};
    EXPECT_NE(v1, v2);
}

TEST_F(Vector2Tests, shouldProperlyCheckIfOneVectorHasGreaterValuesThanOther)
{
    Vector2 v1{5.f, 15.f};
    Vector2 v2{10.f, 10.f};
    EXPECT_TRUE(v1<v2);
}

} // roen::data_structure