#ifndef ROEN_LUA_LUA_CALLABLE_HPP
#define ROEN_LUA_LUA_CALLABLE_HPP

#include <filesystem>
#include <string_view>

#include <sol2/sol.hpp>

namespace roen::lua
{

class LuaCallable
{
public:
    LuaCallable() = default;

    template <typename... Args>
    LuaCallable(sol::state& luaState, std::string_view className, std::string_view fnName,
                const std::filesystem::path& script, Args&&... args);

    template <typename... Args>
    LuaCallable(sol::state& luaState, std::string_view className, std::string_view fnName,
                std::string_view script, Args&&... args);

    template <typename... Args>
    void call(Args&&... args);

    template <typename... Args>
    void operator()(Args&&... args);

    void registerAsGlobal(sol::state& luaState, std::string_view name);

private:
    friend class LuaManager;
    sol::table self_;
    sol::function function_;
};

}  // namespace roen::lua

/*
 * Template implementation
 */

#include <log/Logger.hpp>

namespace roen::lua
{

template <typename... Args>
LuaCallable::LuaCallable(sol::state& luaState, std::string_view className, std::string_view fnName,
                         const std::filesystem::path& script, Args&&... args)
{
    try
    {
        luaState.script_file(script);
    }
    catch (const sol::error& e)
    {
        APP_WARN("Error loading script: {}", script.c_str());
        return;
    }

    sol::table classTable = luaState[className];
    if (not classTable.valid())
    {
        APP_WARN("Class: {} not found in script: {}", className, script.c_str());
        return;
    }

    constexpr std::string_view constructorName{"new"};

    sol::function constructor = classTable[constructorName];
    if (not constructor.valid())
    {
        APP_WARN("Constructor \"{}\" not found in class \"{}\"", constructorName, className);
        return;
    }

    sol::object classInstance = constructor(classTable, std::forward<Args>(args)...);
    if (not classInstance.valid())
    {
        APP_WARN(
            "Constructor \"{}\" for class \"{}\" in script \"{}\" failed to create class instance",
            constructorName, className, script.c_str());
        return;
    }

    self_ = classInstance.as<sol::table>();

    function_ = self_[fnName];
    if (not function_.valid())
    {
        APP_WARN("Function \"{}\" not found for class \"{}\" in script \"{}\"", fnName, className,
                 script.c_str());
        return;
    }
}

template <typename... Args>
LuaCallable::LuaCallable(sol::state& luaState, std::string_view className, std::string_view fnName,
                         std::string_view script, Args&&... args)
{
    try
    {
        luaState.script(script);
    }
    catch (const sol::error& e)
    {
        APP_WARN("Error loading script: {}", script);
        return;
    }

    sol::table classTable = luaState[className];
    if (not classTable.valid())
    {
        APP_WARN("Class: {} not found in script: {}", className, script);
        return;
    }

    constexpr std::string_view constructorName{"new"};

    sol::function constructor = classTable[constructorName];
    if (not constructor.valid())
    {
        APP_WARN("Constructor \"{}\" not found in class \"{}\"", constructorName, className);
        return;
    }

    sol::object classInstance = constructor(classTable, std::forward<Args>(args)...);
    if (not classInstance.valid())
    {
        APP_WARN(
            "Constructor \"{}\" for class \"{}\" in script \"{}\" failed to create class instance",
            constructorName, className, script);
        return;
    }

    self_ = classInstance.as<sol::table>();

    function_ = self_[fnName];
    if (not function_.valid())
    {
        APP_WARN("Function \"{}\" not found for class \"{}\" in script \"{}\"", fnName, className,
                 script);
        return;
    }
}

template <typename... Args>
void LuaCallable::call(Args&&... args)
{
    if (function_.valid())
    {
        function_(self_, std::forward<Args>(args)...);
    }
}

template <typename... Args>
void LuaCallable::operator()(Args&&... args)
{
    call(std::forward<Args>(args)...);
}

}  // namespace roen::lua

#endif  // ROEN_LUA_LUA_CALLABLE_HPP