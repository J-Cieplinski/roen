#include <interfaces/Scene.hpp>

#include <lua/experimental/LuaManager.hpp>

namespace roen::interfaces
{

Scene::Scene(manager::GameSceneManager& gameSceneManager, Application* application)
    : application_{application}
    , gameSceneManager_{gameSceneManager}
{
    lua::experimental::LuaManager::Instance().onInit(this);
}

Scene::~Scene()
{
    entityManager_.clear();
    lua::experimental::LuaManager::Instance().onShutdown();
}

ecs::EntityManager& Scene::getEntityManager()
{
    return entityManager_;
}

Application* Scene::getApplication() const
{
    return application_;
}

events::EventQueue& Scene::getEventQueue()
{
    return queue_;
}

void Scene::update()
{
    auto events = queue_.getEvents();
    handler_.handleEvents(events);
    lua::experimental::LuaManager::Instance().update(1);
}

}  // namespace roen::interfaces
