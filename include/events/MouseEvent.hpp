#ifndef ROEN_EVENTS_MOUSE_EVENT_HPP
#define ROEN_EVENTS_MOUSE_EVENT_HPP

#include <events/Event.hpp>

#include <math/Types.hpp>

#include <cstdint>

namespace roen::events
{

struct MouseEvent final : public Event
{
    MouseEvent(math::Vector2 pos, std::int32_t button)
        : position{pos}
        , mouseButton{button}
    {
    }

    EVENT_CLASS_TYPE(MouseEvent)

    math::Vector2 position;
    std::int32_t mouseButton;
};

}  // namespace roen::events

#endif  // ROEN_EVENTS_MOUSE_EVENT_HPP