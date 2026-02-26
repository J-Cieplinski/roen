#ifndef ROEN_LUA_WORKER_HPP
#define ROEN_LUA_WORKER_HPP

#include <lua/CommandSystem.hpp>
#include <lua/ReadOnlyECS.hpp>

#include <atomic>
#include <sol2/sol.hpp>
#include <vector>

namespace roen::lua
{

class LuaScript;

struct WorkItem
{
    LuaScript* script;
    float dt;
};

class LuaWorker
{
public:
    explicit LuaWorker(int worker_id);
    ~LuaWorker();

    void init();

    std::vector<CommandBuffer> executeBatch(
        const std::vector<WorkItem>& scripts, const InputSnapshot& input,
        EntityQueryCache* query_cache, double time,
        const std::unordered_map<Handle, entt::entity>& handle_map);

    void shutdown();

private:
    int worker_id_;
    std::atomic<bool> running_{false};
    sol::state lua_state_;
};

}  // namespace roen::lua

#endif
