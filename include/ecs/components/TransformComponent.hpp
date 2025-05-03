#ifndef ROEN_ECS_COMPONENTS_TRANSFORM_COMPONENT_HPP
#define ROEN_ECS_COMPONENTS_TRANSFORM_COMPONENT_HPP

#include <raymath.h>

namespace roen::ecs::components
{

struct TransformComponent
{
    operator Vector2&()
    {
        return transform;
    }

    operator const Vector2() const
    {
        return transform;
    }

    bool operator==(const TransformComponent& other)
    {
        return transform == other.transform;
    }

    Vector2 transform{Vector2Zero()};
};

}  // namespace roen::ecs::components

#endif  // ROEN_ECS_COMPONENTS_TRANSFORM_COMPONENT_HPP