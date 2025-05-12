#include <interfaces/Scene.hpp>

#include <lua/LuaManager.hpp>

namespace roen::interfaces
{

Scene::Scene(manager::GameSceneManager& gameSceneManager)
    : gameSceneManager_{gameSceneManager}
{
    lua::LuaManager::Instance().onInit(this);
}

ecs::EntityManager& Scene::getEntityManager()
{
    return entityManager_;
}

events::EventQueue& Scene::getEventQueue()
{
    return queue_;
}

}  // namespace roen::interfaces
