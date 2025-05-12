#ifndef ROEN_INTERFACES_SCENE_HPP
#define ROEN_INTERFACES_SCENE_HPP

#include <data_structure/SystemsContainer.hpp>
#include <ecs/EntityManager.hpp>
#include <events/EventQueue.hpp>

namespace roen::manager
{
class GameSceneManager;
}  // namespace roen::manager

namespace roen::interfaces
{

class Scene
{
public:
    explicit Scene(manager::GameSceneManager& gameSceneManager);

    virtual ~Scene() = default;

    ecs::EntityManager& getEntityManager();
    events::EventQueue& getEventQueue();
    virtual void onInit() = 0;
    virtual void handleInput() = 0;
    virtual void render() = 0;
    virtual void update() = 0;
    virtual void obscured() = 0;
    virtual void revealed() = 0;
    virtual void quit() = 0;

protected:
    manager::GameSceneManager& gameSceneManager_;
    ecs::EntityManager entityManager_;
    data_structure::SystemsContainer systems_;
    events::EventQueue queue_;
};

}  // namespace roen::interfaces

#endif  // ROEN_INTERFACES_SCENE_HPP
