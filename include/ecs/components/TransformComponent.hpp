#ifndef ROEN_ECS_COMPONENTS_TRANSFORM_COMPONENT_HPP
#define ROEN_ECS_COMPONENTS_TRANSFORM_COMPONENT_HPP

#include <math/Types.hpp>

namespace roen::ecs::components
{

struct TransformComponent
{
    bool operator==(const TransformComponent& other) const
    {
        return position == other.position and scale == other.scale and rotation == other.rotation
               and zLayer == other.zLayer;
    }

    math::Vector2 position{0, 0};
    math::Vector2 scale{1, 1};
    float rotation{0};
    std::uint8_t zLayer{0};

    math::Matrix2D worldTransform;

    void updateWorldTransform()
    {
        const auto rotationInRadians = rotation * math::TO_RADIANS;
        worldTransform.compose(position, rotationInRadians, scale);
    }

    void applyParent(const math::Matrix2D& parentMatrix)
    {
        worldTransform.applyParent(parentMatrix);
    }
};

}  // namespace roen::ecs::components

#endif  // ROEN_ECS_COMPONENTS_TRANSFORM_COMPONENT_HPP