#ifndef ROEN_EVENTS_EVENT_HPP
#define ROEN_EVENTS_EVENT_HPP

#include <string>

namespace roen::events
{

enum class EventType
{
    MouseEvent,
    KeyEvent
};

#define EVENT_CLASS_TYPE(type)                      \
    static EventType GetStaticType()                \
    {                                               \
        return EventType::type;                     \
    }                                               \
    virtual EventType getEventType() const override \
    {                                               \
        return GetStaticType();                     \
    }                                               \
    virtual const char* getName() const override    \
    {                                               \
        return #type;                               \
    }

class Event
{
public:
    virtual ~Event() = default;

    virtual EventType getEventType() const = 0;
    virtual const char* getName() const = 0;
    virtual std::string ToString() const
    {
        return getName();
    };
};

}  // namespace roen::events

#endif  // ROEN_EVENTS_EVENT_HPP