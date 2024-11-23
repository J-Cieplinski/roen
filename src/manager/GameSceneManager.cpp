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
        scenes_.top()->quit();
        poppedScenes_.emplace_back(std::move(scenes_.top()));
        scenes_.pop();
    }

    scenes_.push(std::move(scene));
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
    if(!poppedScenes_.empty())
    {
        poppedScenes_.clear();
        scenes_.top()->revealed();
    }
}

} // roen::manager