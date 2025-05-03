#include <lua/LuaCallable.hpp>

#include <log/Logger.hpp>

namespace roen::lua
{

void LuaCallable::registerAsGlobal(sol::state& luaState, std::string_view name)
{
    luaState[name] = self_;
}

}  // namespace roen::lua
