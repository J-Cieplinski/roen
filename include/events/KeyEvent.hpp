#ifndef ROEN_EVENTS_KEY_EVENT_HPP
#define ROEN_EVENTS_KEY_EVENT_HPP

#include <events/Event.hpp>

#include <core/KeyCodes.hpp>

#include <cstdint>

namespace roen::events
{

struct KeyEvent final : public Event
{
    KeyEvent(roen::KeyCodes::Key key)
        : key{key}
    {
    }

    EVENT_CLASS_TYPE(KeyEvent)

    roen::KeyCodes::Key key;
};

}  // namespace roen::events

#endif  // ROEN_EVENTS_KEY_EVENT_HPP