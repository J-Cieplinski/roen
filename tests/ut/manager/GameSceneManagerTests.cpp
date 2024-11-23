#include <manager/GameSceneManager.hpp>

#include <interfaces/IScene.hpp>

#include <gmock/gmock.h>

namespace roen
{

namespace interfaces
{

class SceneMock final : public IScene
{
public:
    explicit SceneMock(manager::GameSceneManager& manager) : IScene(manager) {};

    MOCK_METHOD(void, handleInput, (), (override));
    MOCK_METHOD(void, render, (), (override));
    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, obscured, (), (override));
    MOCK_METHOD(void, revealed, (), (override));
    MOCK_METHOD(void, quit, (), (override));
};

} // interfaces

namespace manager
{

class GameSceneManagerTests : public ::testing::Test
{
protected:
    GameSceneManager manager_;
};

TEST_F(GameSceneManagerTests, push_ShouldCorrectlyPushScene)
{
    auto scene = std::make_unique<interfaces::SceneMock>(manager_);
    auto* scenePtr = scene.get();

    manager_.push(std::move(scene));

    EXPECT_EQ(&manager_.getCurrentScene(), scenePtr);
}

TEST_F(GameSceneManagerTests, push_ShouldCallRevealedOnPushedScene)
{
    auto scene = std::make_unique<interfaces::SceneMock>(manager_);
    EXPECT_CALL(*scene, revealed());

    manager_.push(std::move(scene));
}

TEST_F(GameSceneManagerTests, push_ShouldCallObscuredOnPreviouslyTopScene)
{
    auto topScene = std::make_unique<interfaces::SceneMock>(manager_);
    EXPECT_CALL(*topScene, obscured());

    manager_.push(std::move(topScene));
    manager_.push(std::make_unique<interfaces::SceneMock>(manager_));
    manager_.push(std::make_unique<interfaces::SceneMock>(manager_));
}

TEST_F(GameSceneManagerTests, pop_ShouldThrowIfNoScenesPresent)
{
    EXPECT_THROW(manager_.pop(), std::out_of_range);
}

TEST_F(GameSceneManagerTests, pop_ShouldCorrectlyPopTopScene)
{
    auto sceneOne = std::make_unique<interfaces::SceneMock>(manager_);
    auto* sceneOnePtr = sceneOne.get();
    auto topScene = std::make_unique<interfaces::SceneMock>(manager_);
    auto* topScenePtr = topScene.get();

    manager_.push(std::move(sceneOne));
    manager_.push(std::move(topScene));

    EXPECT_EQ(&manager_.getCurrentScene(), topScenePtr);

    manager_.pop();
    EXPECT_EQ(&manager_.getCurrentScene(), sceneOnePtr);
}

TEST_F(GameSceneManagerTests, pop_ShouldCallRevealOnNewTopScene)
{
    auto sceneOne = std::make_unique<interfaces::SceneMock>(manager_);
    EXPECT_CALL(*sceneOne, revealed())
        .Times(2); //Once on push, once after pop

    manager_.push(std::move(sceneOne));
    manager_.push(std::make_unique<interfaces::SceneMock>(manager_));

    manager_.pop();
}

TEST_F(GameSceneManagerTests, getCurrentScene_ShouldThrowIfNoScenesPresent)
{
    EXPECT_THROW(manager_.getCurrentScene(), std::out_of_range);
}

TEST_F(GameSceneManagerTests, getCurrentScene_ShouldReturnTopScene)
{
    auto sceneOne = std::make_unique<interfaces::SceneMock>(manager_);
    auto* sceneOnePtr = sceneOne.get();
    auto topScene = std::make_unique<interfaces::SceneMock>(manager_);
    auto* topScenePtr = topScene.get();

    manager_.push(std::move(sceneOne));
    manager_.push(std::move(topScene));

    auto& currentScene = manager_.getCurrentScene();

    EXPECT_EQ(&currentScene, topScenePtr);
    EXPECT_NE(&currentScene, sceneOnePtr);
}

} // manager

} // roen