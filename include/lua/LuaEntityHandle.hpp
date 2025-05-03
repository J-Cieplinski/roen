#ifndef ROEN_LUA_LUA_ENTITY_HANDLE_HPP
#define ROEN_LUA_LUA_ENTITY_HANDLE_HPP

#include <ecs/Entity.hpp>

#include <cstdint>

namespace roen::lua
{

class LuaEntityHandle
{
public:
    LuaEntityHandle(ecs::Entity& entity);
    void setTransform(std::uint32_t x, std::uint32_t y);

private:
    ecs::Entity& entity_;
};

}  // namespace roen::lua

#endif  // ROEN_LUA_LUA_ENTITY_HANDLE_HPP
