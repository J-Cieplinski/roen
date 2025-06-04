#ifndef ROEN_LUA_LUA_SCRIPT_HPP
#define ROEN_LUA_LUA_SCRIPT_HPP

#include <filesystem>
#include <string_view>

#include <sol2/sol.hpp>

namespace roen::ecs
{
class Entity;
}

namespace roen::lua
{

class LuaScript
{
public:
    LuaScript(ecs::Entity* entity = nullptr);

    void loadScript(const std::filesystem::path& filename);
    void onUpdate(float dt) const;
    ecs::Entity* getEntity() const;

private:
    std::unique_ptr<sol::protected_function> onUpdate_;
    std::unique_ptr<sol::protected_function> onInit_;

    sol::environment env_;
    ecs::Entity* entity_;
};

}  // namespace roen::lua

#endif  // ROEN_LUA_LUA_SCRIPT_HPP