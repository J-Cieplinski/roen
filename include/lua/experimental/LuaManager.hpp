#ifndef ROEN_LUA_EXPERIMENTAL_LUA_MANAGER_HPP
#define ROEN_LUA_EXPERIMENTAL_LUA_MANAGER_HPP

#include <atomic>
#include <coro/coro.hpp>
#include <memory>
#include <sol2/sol.hpp>
#include <vector>
#include "CommandSystem.hpp"
#include "LuaWorker.hpp"
#include "ReadOnlyECS.hpp"
#include "coro/task.hpp"
#include "coro/thread_pool.hpp"

namespace roen::interfaces
{
class Scene;
}

namespace roen::lua::experimental
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

    // Coroutine to execute batch of scripts on a worker
    coro::task<std::vector<CommandBuffer>> executeBatchAsync(int worker_idx,
                                                             const std::vector<WorkItem>& batch,
                                                             const InputSnapshot& input,
                                                             double time) const;

    inline static std::unique_ptr<LuaManager> instance_;

    // Main thread state (for script loading only)
    sol::state lua_;

    // Worker VMs (not threads themselves, used by coroutines)
    std::vector<std::unique_ptr<LuaWorker>> workers_;

    // libcoro thread pool for executing script coroutines
    std::unique_ptr<coro::thread_pool> thread_pool_;

    interfaces::Scene* scene_ = nullptr;
    double elapsed_time_ = 0;
    std::unordered_map<Handle, entt::entity> handle_map_;

    // Shared query cache (thread-safe)
    std::unique_ptr<EntityQueryCache> query_cache_;
};

}  // namespace roen::lua::experimental

#endif
