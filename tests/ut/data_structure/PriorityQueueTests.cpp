#include <data_structure/PriorityQueue.hpp>

#include <gtest/gtest.h>

namespace roen::data_structure
{

class PriorityQueueTests : public testing::Test
{
};

TEST_F(PriorityQueueTests, empty_ShouldReturnTrueForEmptyQueue)
{
    PriorityQueue<int, int> queue;

    EXPECT_TRUE(queue.empty());
}

TEST_F(PriorityQueueTests, insert_ShouldInsertElements)
{
    PriorityQueue<int, int> queue;
    queue.insert({1, 1});

    EXPECT_FALSE(queue.empty());
}

TEST_F(PriorityQueueTests, get_ShouldReturnElementWithHighestPriority)
{
    constexpr int EXPECTED_ELEMENT {5};
    PriorityQueue<int, int> queue;
    queue.insert({1,5});
    queue.insert({2,3});

    EXPECT_EQ(queue.get(), EXPECTED_ELEMENT);
}

TEST_F(PriorityQueueTests, get_ShouldPopElement)
{
    constexpr int EXPECTED_ELEMENT {5};
    PriorityQueue<int, int> queue;
    queue.insert({1,5});

    EXPECT_EQ(queue.get(), EXPECTED_ELEMENT);
    EXPECT_TRUE(queue.empty());
}

} // roen::data_structure