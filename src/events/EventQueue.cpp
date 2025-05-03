#include <events/EventQueue.hpp>

namespace roen::events
{

void EventQueue::clear()
{
    events_.clear();
}

std::vector<std::shared_ptr<Event>> EventQueue::getEvents()
{
    return events_;
}

}  // namespace roen::events
