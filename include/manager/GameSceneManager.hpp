#ifndef ROEN_MANAGER_GAMESCENEMANAGER_HPP
#define ROEN_MANAGER_GAMESCENEMANAGER_HPP

#include <memory>
#include <stack>
#include <vector>

namespace roen
{

namespace interfaces
{
class Scene;
}  // namespace interfaces

namespace manager
{

class GameSceneManager
{
public:
    void shutdown();

    void push(std::unique_ptr<interfaces::Scene> scene);
    void pop();
    void switchScene(std::unique_ptr<interfaces::Scene> scene);
    void update();
    interfaces::Scene& getCurrentScene();

private:
    std::stack<std::unique_ptr<interfaces::Scene>> scenes_;
    std::vector<std::unique_ptr<interfaces::Scene>> poppedScenes_;
    std::vector<std::unique_ptr<interfaces::Scene>> delayedScenes_;
};

}  // namespace manager

}  // namespace roen

#endif  // ROEN_MANAGER_GAMESCENEMANAGER_HPP