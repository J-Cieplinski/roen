#ifndef ROEN_ECS_LIFECYCLE_SYSTEM_HPP
#define ROEN_ECS_LIFECYCLE_SYSTEM_HPP

#include <interfaces/Scene.hpp>
#include <vector>

namespace roen::ecs
{

class LifecycleSystem
{
public:
    explicit LifecycleSystem(interfaces::Scene* scene)
        : scene_(scene)
    {
    }

    std::vector<entt::entity> update(float dt) const;

private:
    interfaces::Scene* scene_;
};

}  // namespace roen::ecs

#endif
