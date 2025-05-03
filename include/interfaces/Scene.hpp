#ifndef ROEN_INTERFACES_SCENE_HPP
#define ROEN_INTERFACES_SCENE_HPP

#include <data_structure/SystemsContainer.hpp>

namespace roen::manager
{
class GameSceneManager;
}  // namespace roen::manager

namespace roen::interfaces
{

class Scene
{
public:
    explicit Scene(manager::GameSceneManager& gameSceneManager);

    virtual ~Scene() = default;

    virtual void onInit();
    virtual void handleInput() = 0;
    virtual void render() = 0;
    virtual void update() = 0;
    virtual void obscured() = 0;
    virtual void revealed() = 0;
    virtual void quit() = 0;

protected:
    manager::GameSceneManager& gameSceneManager_;
    data_structure::SystemsContainer systems_;
};

}  // namespace roen::interfaces

#endif  // ROEN_INTERFACES_SCENE_HPP
