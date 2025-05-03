#ifndef ROEN_ECS_COMPONENTS_GRAPHICS_COMPONENT_HPP
#define ROEN_ECS_COMPONENTS_GRAPHICS_COMPONENT_HPP

#include <Utils.hpp>

#include <raylib.h>

#include <cstdint>
#include <string>

namespace roen::ecs::components
{

struct GraphicsComponent
{
    GraphicsComponent()
        : srcRectangle{0, 0, 0, 0}
        , guid{0}
        , zLayer{0}
    {
    }

    GraphicsComponent(const std::string& str, Rectangle srcRectangle)
        : GraphicsComponent{str, srcRectangle, 0}
    {
    }

    GraphicsComponent(const std::string& str, Rectangle srcRectangle, std::uint8_t zLayer)
        : srcRectangle{srcRectangle}
        , guid{roen::hashString(str)}
        , zLayer{zLayer}
    {
    }

    operator Rectangle&()
    {
        return srcRectangle;
    }

    operator const Rectangle() const
    {
        return srcRectangle;
    }

    bool operator==(const GraphicsComponent& other)
    {
        return srcRectangle.height == other.srcRectangle.height
               and srcRectangle.width == other.srcRectangle.width
               and srcRectangle.x == other.srcRectangle.x and srcRectangle.y == other.srcRectangle.y
               and zLayer == other.zLayer and guid == other.guid;
    }

    Rectangle srcRectangle;
    std::uint8_t zLayer;
    std::uint64_t guid;
};

}  // namespace roen::ecs::components

#endif  // ROEN_ECS_COMPONENTS_GRAPHICS_COMPONENT_HPP