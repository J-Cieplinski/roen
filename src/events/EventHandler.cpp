#include <events/EventHandler.hpp>

namespace roen::events
{

void EventHandler::handleEvents(std::vector<std::shared_ptr<Event>> events)
{
    for (const auto& event : events)
    {
        std::type_index type = typeid(*event);
        auto it = callbacks_.find(type);
        if (it != callbacks_.end())
        {
            for (auto& cb : it->second)
            {
                cb(*event);
            }
        }
    }
}

}  // namespace roen::events
