#include <interfaces/Scene.hpp>

namespace roen::interfaces
{

Scene::Scene(manager::GameSceneManager& gameSceneManager)
    : gameSceneManager_{gameSceneManager}
{
}

void Scene::onInit()
{
}

}  // namespace roen::interfaces
