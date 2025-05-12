#include <events/EventQueue.hpp>

namespace roen::events
{

void EventQueue::clear()
{
    events_.clear();
}

std::vector<std::shared_ptr<Event>> EventQueue::getEvents()
{
    std::vector<std::shared_ptr<Event>> eventsTmp;
    std::swap(events_, eventsTmp);
    return std::move(eventsTmp);
}

}  // namespace roen::events
