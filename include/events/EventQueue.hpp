#ifndef ROEN_EVENTS_EVENT_QUEUE_HPP
#define ROEN_EVENTS_EVENT_QUEUE_HPP

#include <events/Event.hpp>

#include <memory>
#include <vector>

namespace roen::events
{

class EventQueue
{
public:
    template <typename EventType, typename... Args>
    void enqueue(Args... args);
    void clear();
    std::vector<std::shared_ptr<Event>> getEvents();

private:
    std::vector<std::shared_ptr<Event>> events_;
};

}  // namespace roen::events

/*
 * Template implementation
 */

namespace roen::events
{

template <typename EventType, typename... Args>
void EventQueue::enqueue(Args... args)
{
    events_.push_back(std::make_shared<EventType>(std::forward<Args>(args)...));
}

}  // namespace roen::events

#endif  // ROEN_EVENTS_EVENT_QUEUE_HPP