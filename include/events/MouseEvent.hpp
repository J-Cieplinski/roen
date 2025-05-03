#ifndef ROEN_EVENTS_MOUSE_EVENT_HPP
#define ROEN_EVENTS_MOUSE_EVENT_HPP

#include <events/Event.hpp>

#include <raymath.h>

#include <cstdint>

namespace roen::events
{

struct MouseEvent final : public Event
{
    MouseEvent(Vector2 pos, std::int32_t button)
        : position{pos}
        , mouseButton{button}
    {
    }

    EVENT_CLASS_TYPE(MouseEvent)

    Vector2 position;
    std::int32_t mouseButton;
};

}  // namespace roen::events

#endif  // ROEN_EVENTS_MOUSE_EVENT_HPP