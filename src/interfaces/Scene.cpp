#include <interfaces/Scene.hpp>

#include <lua/LuaManager.hpp>

namespace roen::interfaces
{

Scene::Scene(manager::GameSceneManager& gameSceneManager)
    : gameSceneManager_{gameSceneManager}
{
}

void Scene::onInit()
{
    lua::LuaManager::Instance().onInit(this);
}

ecs::EntityManager& Scene::getEntityManager()
{
    return entityManager_;
}

}  // namespace roen::interfaces
