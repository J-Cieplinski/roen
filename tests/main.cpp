#include <gtest/gtest.h>

#include <log/Logger.hpp>

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    roen::log::Logger::Init();
    return RUN_ALL_TESTS();
}