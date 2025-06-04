#include <interfaces/Scene.hpp>

#include <lua/LuaManager.hpp>

namespace roen::interfaces
{

Scene::Scene(manager::GameSceneManager& gameSceneManager, Application* application)
    : application_{application}
    , gameSceneManager_{gameSceneManager}
{
    lua::LuaManager::Instance().onInit(this);
}

Scene::~Scene()
{
    lua::LuaManager::Instance().onShutdown();
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
    lua::LuaManager::Instance().callEventHandler(events);
    lua::LuaManager::Instance().update();
}

}  // namespace roen::interfaces
