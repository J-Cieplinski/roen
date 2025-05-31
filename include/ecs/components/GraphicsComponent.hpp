#ifndef ROEN_ECS_COMPONENTS_GRAPHICS_COMPONENT_HPP
#define ROEN_ECS_COMPONENTS_GRAPHICS_COMPONENT_HPP

#include <Utils.hpp>

#include <math/Types.hpp>

#include <cstdint>
#include <string>

namespace roen::ecs::components
{

struct GraphicsComponent
{
    GraphicsComponent()
        : srcRectangle{0, 0, 0, 0}
        , guid{""}
    {
    }

    GraphicsComponent(std::string_view str, math::Rectangle srcRectangle)
        : GraphicsComponent{str, srcRectangle, 0}
    {
        SDK_TRACE("Constructed GraphicsComponent with str: {}", str);
    }

    GraphicsComponent(std::string_view str, math::Rectangle srcRectangle, std::uint8_t zLayer)
        : srcRectangle{srcRectangle}
        , guid{str}
    {
    }

    operator math::Rectangle&() { return srcRectangle; }

    operator const math::Rectangle() const { return srcRectangle; }

    bool operator==(const GraphicsComponent& other) const
    {
        return srcRectangle == other.srcRectangle and guid == other.guid;
    }

    math::Rectangle srcRectangle;
    std::string guid;
};

}  // namespace roen::ecs::components

#endif  // ROEN_ECS_COMPONENTS_GRAPHICS_COMPONENT_HPP