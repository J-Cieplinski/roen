#ifndef ROEN_LUA_SCRIPTS_LUA_EVENT_HANDLER_HPP
#define ROEN_LUA_SCRIPTS_LUA_EVENT_HANDLER_HPP

#include <string_view>

namespace roen::lua::scripts
{

constexpr std::string_view EventHandlerScript =
    R"(
    EventHandler = {}

    EventHandler.__index = EventHandler

    function EventHandler:create()
        local obj = setmetatable({}, self)
        obj.callbacks = {}
        return obj
    end

    function EventHandler:subscribe(event, callback)
        Logger.Info("Subscribe called with event: " .. event)
        if not self.callbacks[event] then
            self.callbacks[event] = {}
        end

        table.insert(self.callbacks[event], callback)
    end

    function EventHandler:handleEvents(events)
        for _, event in ipairs(events) do
            local eventCallbacks = self.callbacks[event:getName()]
            if eventCallbacks then
                for _, callback in ipairs(eventCallbacks) do
                    callback(event)
                end
            end
        end
    end
    )";

}  // namespace roen::lua::scripts

#endif  // ROEN_LUA_SCRIPTS_LUA_EVENT_HANDLER_HPP