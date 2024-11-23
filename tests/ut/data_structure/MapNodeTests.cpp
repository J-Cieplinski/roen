#include <data_structure/MapNode.hpp>

#include <gtest/gtest.h>

namespace roen::data_structure
{

namespace
{
constexpr std::uint32_t NODE_COST {1};
} //

class MapNodeTests : public testing::Test
{
};

TEST_F(MapNodeTests, cost_ShouldReturnCorrectNodeCost)
{
    MapNode node({0, 0}, {16, 16}, NODE_COST);

    EXPECT_EQ(node.cost(), NODE_COST);
}

TEST_F(MapNodeTests, contains_ShouldReturnTrueIfEntityIsWithinNode)
{
    MapNode node({0, 0}, {16, 16}, NODE_COST);

    constexpr Vector2f CONTAINED_ENTITY {14.5f, 0.f};

    EXPECT_TRUE(node.contains(CONTAINED_ENTITY));
}

TEST_F(MapNodeTests, contains_ShouldReturnFalseIfEntityIsNotWithinNode)
{
    MapNode node({0, 0}, {16, 16}, NODE_COST);

    constexpr Vector2f NOT_CONTAINED_ENTITY {16.5f, 0.f};

    EXPECT_FALSE(node.contains(NOT_CONTAINED_ENTITY));
}

TEST_F(MapNodeTests, operatorMinus_ShouldCorrectlyReturnDistanceBetweenNodes)
{
    MapNode node({5, 22}, {16, 16}, NODE_COST);
    MapNode secondNode({34, 128}, {16, 16}, NODE_COST);

    EXPECT_EQ(node - secondNode, Vector2f(-29.f, -106.f));
    EXPECT_EQ(secondNode - node, Vector2f(29.f, 106.f));
}

} // roen::data_structure