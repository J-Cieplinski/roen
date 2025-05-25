#ifndef ROEN_INTERFACES_SCENE_HPP
#define ROEN_INTERFACES_SCENE_HPP

#include <data_structure/SystemsContainer.hpp>
#include <ecs/EntityManager.hpp>
#include <events/EventQueue.hpp>

namespace roen
{

class Application;

namespace manager
{
class GameSceneManager;
}
}  // namespace roen

namespace roen::interfaces
{

class Scene
{
public:
    explicit Scene(manager::GameSceneManager& gameSceneManager, Application* application);

    virtual ~Scene() = default;

    ecs::EntityManager& getEntityManager();
    Application* getApplication() const;
    events::EventQueue& getEventQueue();
    virtual void onInit() = 0;
    virtual void handleInput() = 0;
    virtual void update() = 0;
    virtual void obscured() = 0;
    virtual void revealed() = 0;
    virtual void quit() = 0;

protected:
    Application* application_;
    manager::GameSceneManager& gameSceneManager_;
    ecs::EntityManager entityManager_;
    data_structure::SystemsContainer systems_;
    events::EventQueue queue_;
};

}  // namespace roen::interfaces

#endif  // ROEN_INTERFACES_SCENE_HPP
