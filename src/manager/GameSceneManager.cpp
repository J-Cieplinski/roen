#include <manager/GameSceneManager.hpp>

#include <interfaces/IScene.hpp>

#include <stdexcept>

namespace roen::manager
{

void GameSceneManager::shutdown()
{
    while(not scenes_.empty())
    {
        scenes_.pop();
    }
}

void GameSceneManager::push(std::unique_ptr<interfaces::IScene> scene)
{
    if(!scenes_.empty())
    {
        scenes_.top()->obscured();
    }

    scene->revealed();
    scenes_.push(std::move(scene));
}

void GameSceneManager::switchScene(std::unique_ptr<interfaces::IScene> scene)
{
    if(!scenes_.empty())
    {
        poppedScenes_.emplace_back(std::move(scenes_.top()));
        scenes_.pop();
    }

    delayedScenes_.emplace_back(std::move(scene));
}

void GameSceneManager::pop()
{
    if(scenes_.empty())
    {
        throw std::out_of_range("There are no game scenes present in the manager");
    }

    scenes_.pop();

    if(!scenes_.empty())
    {
        scenes_.top()->revealed();
    }
}

interfaces::IScene& GameSceneManager::getCurrentScene()
{
    if(scenes_.empty())
    {
        throw std::out_of_range("There are no game scenes present in the manager");
    }

    return static_cast<interfaces::IScene&>(*(scenes_.top()));
}

void GameSceneManager::update()
{
    for(auto& poppedScene : poppedScenes_)
    {
        poppedScene->quit();
    }
    poppedScenes_.clear();

    for(auto& scene : delayedScenes_)
    {
        scenes_.push(std::move(scene));
    }

    if(!delayedScenes_.empty())
    {
        scenes_.top()->revealed();
    }

    delayedScenes_.clear();
}

} // roen::manager