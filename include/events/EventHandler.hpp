#ifndef ROEN_EVENTS_EVENT_HANDLER_HPP
#define ROEN_EVENTS_EVENT_HANDLER_HPP

#include <events/Event.hpp>

#include <functional>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace roen::events
{

class EventHandler
{
    using callback = std::function<void(Event& event)>;

public:
    void handleEvents(std::vector<std::shared_ptr<Event>> events);

    template <typename EventType>
    void subscribe(std::function<void(EventType& event)> cb);

private:
    std::unordered_map<std::type_index, std::vector<callback>> callbacks_;
};

}  // namespace roen::events

/*
 * Template implementation
 */

namespace roen::events
{

template <typename EventType>
void EventHandler::subscribe(std::function<void(EventType& event)> cb)
{
    std::type_index type = typeid(EventType);
    callbacks_[type].emplace_back([cb](Event& event) { cb(static_cast<EventType&>(event)); });
}

}  // namespace roen::events

#endif  // ROEN_EVENTS_EVENT_HANDLER_HPP