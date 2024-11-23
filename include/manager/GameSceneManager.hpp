#ifndef ROEN_MANAGER_GAMESCENEMANAGER_HPP
#define ROEN_MANAGER_GAMESCENEMANAGER_HPP

#include <interfaces/IScene.hpp>

#include <memory>
#include <stack>

namespace roen::manager
{

class GameSceneManager
{
public:
    void shutdown();

    void push(std::unique_ptr<interfaces::IScene> scene);
    void pop();
    void switchScene(std::unique_ptr<interfaces::IScene> scene);
    void update();
    [[nodiscard]] interfaces::IScene& getCurrentScene();
private:
    std::stack<std::unique_ptr<interfaces::IScene>> scenes_;
    std::vector<std::unique_ptr<interfaces::IScene>> poppedScenes_;
};

} // roen::manager

#endif //ROEN_MANAGER_GAMESCENEMANAGER_HPP