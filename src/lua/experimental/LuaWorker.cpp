#include <core/KeyCodes.hpp>
#include <lua/experimental/LuaApiBindings.hpp>
#include <lua/experimental/LuaScript.hpp>
#include <lua/experimental/LuaWorker.hpp>

namespace roen::lua::experimental
{

LuaWorker::LuaWorker(int worker_id)
    : worker_id_(worker_id)
{
    lua_state_.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::table,
                              sol::lib::os, sol::lib::string);
    bindGlobalAPI(lua_state_);
}

LuaWorker::~LuaWorker()
{
    shutdown();
}

void LuaWorker::init()
{
    running_ = true;
}

std::vector<CommandBuffer> LuaWorker::executeBatch(
    const std::vector<WorkItem>& scripts, const InputSnapshot& input, EntityQueryCache* query_cache,
    double time, const std::unordered_map<Handle, entt::entity>& handle_map)
{
    std::vector<CommandBuffer> results;
    results.reserve(scripts.size());

    SceneQuery query(query_cache);

    for (auto& item : scripts)
    {
        if (not item.script->isInitialized())
        {
            item.script->initializeScript(lua_state_);
        }

        CommandBuffer cmds;
        item.script->onUpdate(item.dt, input, query, &cmds, time, handle_map);
        results.push_back(std::move(cmds));
    }

    return results;
}

void LuaWorker::shutdown()
{
    running_ = false;
}

}  // namespace roen::lua::experimental
