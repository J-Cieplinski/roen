#ifndef ROEN_ECS_COMPONENTS_TRANSFORM_COMPONENT_HPP
#define ROEN_ECS_COMPONENTS_TRANSFORM_COMPONENT_HPP

#include <math/Types.hpp>

namespace roen::ecs::components
{

struct TransformComponent
{
    operator math::Vector2&() { return transform; }

    operator const math::Vector2() const { return transform; }

    bool operator==(const TransformComponent& other) const { return transform == other.transform; }

    math::Vector2 transform;
};

}  // namespace roen::ecs::components

#endif  // ROEN_ECS_COMPONENTS_TRANSFORM_COMPONENT_HPP