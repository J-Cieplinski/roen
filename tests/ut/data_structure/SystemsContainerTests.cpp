#include <data_structure/SystemsContainer.hpp>
#include <interfaces/ISystem.hpp>

#include <entt/entity/registry.hpp>

#include <gmock/gmock.h>

namespace roen
{

namespace interfaces
{

class SystemMock final : public ISystem
{
public:
    explicit SystemMock(entt::registry& entityManager) : ISystem(entityManager) {};
};

class SystemStub final : public ISystem
{
public:
    explicit SystemStub(entt::registry& entityManager, int id = 0)
        : ISystem(entityManager)
        , systemId(id)
    {
    };

    int systemId;
};

} // interfaces

namespace data_structure
{

class SystemsContainerTests : public ::testing::Test
{
protected:
    entt::registry registry_;
};

TEST_F(SystemsContainerTests, hasSystem_ShouldReturnFalseWhenSystemIsNotPresent)
{
    SystemsContainer sut {};
    EXPECT_FALSE(sut.hasSystem<interfaces::SystemStub>());
}

TEST_F(SystemsContainerTests, hasSystem_ShouldReturnTrueWhenSystemIsPresent)
{
    SystemsContainer sut {};

    sut.add<interfaces::SystemStub>(registry_);
    EXPECT_TRUE(sut.hasSystem<interfaces::SystemStub>());
}

TEST_F(SystemsContainerTests, add_ShouldAddSystem)
{
    SystemsContainer sut {};

    sut.add<interfaces::SystemStub>(registry_);
    EXPECT_TRUE(sut.hasSystem<interfaces::SystemStub>());
}

TEST_F(SystemsContainerTests, add_ShouldNotReplaceSystemIfTheSameTypeIsAlreadyPresent)
{
    SystemsContainer sut {};
    constexpr int firstSystemId {1};
    constexpr int secondSystemId {2};

    sut.add<interfaces::SystemStub>(registry_, firstSystemId);
    EXPECT_EQ(sut.get<interfaces::SystemStub>().systemId, firstSystemId);

    sut.add<interfaces::SystemStub>(registry_, secondSystemId);
    EXPECT_EQ(sut.get<interfaces::SystemStub>().systemId, firstSystemId);
}

TEST_F(SystemsContainerTests, remove_ShouldRemoveSystem)
{
    SystemsContainer sut {};

    sut.add<interfaces::SystemStub>(registry_);
    EXPECT_TRUE(sut.hasSystem<interfaces::SystemStub>());

    sut.remove<interfaces::SystemStub>();
    EXPECT_FALSE(sut.hasSystem<interfaces::SystemStub>());
}

TEST_F(SystemsContainerTests, remove_ShouldDoNothingIfSystemIsNotPresent)
{
    SystemsContainer sut {};

    sut.add<interfaces::SystemStub>(registry_);
    EXPECT_TRUE(sut.hasSystem<interfaces::SystemStub>());

    sut.remove<interfaces::SystemMock>();
    EXPECT_TRUE(sut.hasSystem<interfaces::SystemStub>());
}

TEST_F(SystemsContainerTests, get_ShouldReturnSystem)
{
    SystemsContainer sut {};

    sut.add<interfaces::SystemStub>(registry_);
    EXPECT_THAT(std::type_index(typeid(sut.get<interfaces::SystemStub>())), std::type_index(typeid(interfaces::SystemStub)));
}

TEST_F(SystemsContainerTests, get_ShouldThrowIfSystemNotPresent)
{
    SystemsContainer sut {};
    EXPECT_THROW(sut.get<interfaces::SystemStub>(), std::out_of_range);
}

TEST_F(SystemsContainerTests, ShouldBeAbleToIterateOverSystems)
{
    constexpr int expectedNumOfLoops{2};
    SystemsContainer sut {};
    sut.add<interfaces::SystemStub>(registry_);
    sut.add<interfaces::SystemMock>(registry_);

    int numOfLoops {0};
    for(const auto& system : sut)
    {
        ++numOfLoops;
    }

    EXPECT_THAT(numOfLoops, expectedNumOfLoops);
}

} // data_structure

} // roen