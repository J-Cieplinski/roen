#ifndef ROEN_LUA_EXPERIMENTAL_LUA_MANAGER_HPP
#define ROEN_LUA_EXPERIMENTAL_LUA_MANAGER_HPP

#include <lua/CommandSystem.hpp>
#include <lua/LuaWorker.hpp>
#include <lua/ReadOnlyECS.hpp>

#include <memory>

#include <coro/task.hpp>
#include <coro/thread_pool.hpp>
#include <sol2/sol.hpp>

#include "ecs/systems/LifecycleSystem.hpp"

namespace roen::interfaces
{
class Scene;
}

namespace roen::lua
{

class LuaManager
{
public:
    static LuaManager& Instance();

    LuaManager(LuaManager&) = delete;
    LuaManager& operator=(const LuaManager&) = delete;
    ~LuaManager();

    sol::state_view getState() const;

    // Initialize with worker count (uses libcoro thread pool)
    void onInit(interfaces::Scene* scene, int num_workers = 4);
    void onShutdown();
    void update(float dt);

private:
    LuaManager() = default;

    void initMainState();
    void initWorkers(int num_workers);

    coro::task<std::vector<CommandBuffer>> executeBatchAsync(int worker_idx,
                                                             const std::vector<WorkItem>& batch,
                                                             const InputSnapshot& input,
                                                             double time) const;

    inline static std::unique_ptr<LuaManager> instance_;

    sol::state lua_;

    std::vector<std::unique_ptr<LuaWorker>> workers_;

    std::unique_ptr<coro::thread_pool> thread_pool_;

    interfaces::Scene* scene_ = nullptr;
    double elapsed_time_ = 0;
    std::unordered_map<Handle, entt::entity> handle_map_;

    std::unique_ptr<EntityQueryCache> query_cache_;
    std::unique_ptr<ecs::LifecycleSystem> lifecycle_system_;
};

}  // namespace roen::lua

#endif
