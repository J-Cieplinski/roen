#include <core/KeyCodes.hpp>
#include <ecs/components/LifecycleComponent.hpp>
#include <interfaces/Scene.hpp>
#include <log/Logger.hpp>
#include <lua/CommandApplicator.hpp>
#include <lua/LuaApiBindings.hpp>
#include <lua/LuaManager.hpp>
#include <lua/LuaScript.hpp>
#include <lua/LuaWorker.hpp>
#include <lua/ReadOnlyECS.hpp>
#include <lua/SnapshotCapture.hpp>

#include <algorithm>

#include <coro/sync_wait.hpp>
#include <coro/thread_pool.hpp>
#include <coro/when_all.hpp>

namespace roen::lua
{

LuaManager::~LuaManager() = default;

LuaManager& LuaManager::Instance()
{
    if (not instance_)
    {
        instance_ = std::unique_ptr<LuaManager>(new LuaManager());
    }
    return *instance_;
}

sol::state_view LuaManager::getState() const
{
    return lua_;
}

void LuaManager::onInit(interfaces::Scene* scene, int num_workers)
{
    scene_ = scene;

    initMainState();

    // Create libcoro thread pool
    thread_pool_ = coro::thread_pool::make_unique(
        coro::thread_pool::options{.thread_count = static_cast<uint32_t>(num_workers)});
    SDK_INFO("Thread pool size: {}", thread_pool_->thread_count());

    initWorkers(num_workers);

    // Create thread-safe query cache
    query_cache_ = std::make_unique<EntityQueryCache>(scene_);

    SDK_INFO("LuaManager initialized with {0} worker threads (libcoro)", num_workers);
}

void LuaManager::initMainState()
{
    lua_.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::table,
                        sol::lib::os, sol::lib::string);

    // Set up script search paths (optional, adjust to your needs)
    constexpr std::string_view scripts_path{"assets/scripts"};
    if (std::filesystem::exists(scripts_path))
    {
        std::string lua_paths;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(scripts_path))
        {
            if (entry.is_directory())
            {
                std::string path = entry.path().string();
                std::ranges::replace(path, '\\', '/');
                lua_paths += path + "/?.lua;";
            }
        }
        std::string current_paths = lua_["package"]["path"];
        lua_["package"]["path"] = lua_paths + current_paths;
    }

    // Bind API in main state (for script loading)
    bindGlobalAPI(lua_);

    // Register math types
    auto vec2 = lua_.new_usertype<math::Vector2>(
        "Vector2", sol::constructors<math::Vector2(float, float)>(), "x", &math::Vector2::x, "y",
        &math::Vector2::y, sol::meta_function::addition, &math::Vector2::operator+,
        sol::meta_function::subtraction, &math::Vector2::operator-);

    auto rect = lua_.new_usertype<math::Rectangle>(
        "Rectangle", sol::constructors<math::Rectangle(float, float, float, float)>(), "x",
        &math::Rectangle::x, "y", &math::Rectangle::y, "width", &math::Rectangle::width, "height",
        &math::Rectangle::height);

    // Register Key enum (add all keys you need)
    std::initializer_list<std::pair<sol::string_view, KeyCodes::Key>> keyItems
        = {{"A", KeyCodes::Key::A},         {"B", KeyCodes::Key::B},
           {"C", KeyCodes::Key::C},         {"D", KeyCodes::Key::D},
           {"E", KeyCodes::Key::E},         {"F", KeyCodes::Key::F},
           {"W", KeyCodes::Key::W},         {"S", KeyCodes::Key::S},
           {"SPACE", KeyCodes::Key::SPACE}, {"ESCAPE", KeyCodes::Key::ESCAPE},
           {"ENTER", KeyCodes::Key::ENTER}, {"UP", KeyCodes::Key::UP},
           {"DOWN", KeyCodes::Key::DOWN},   {"LEFT", KeyCodes::Key::LEFT},
           {"RIGHT", KeyCodes::Key::RIGHT}, {"Z", KeyCodes::Key::Z}};
    lua_.new_enum<KeyCodes::Key, false>("Key", keyItems);

    std::initializer_list<std::pair<sol::string_view, KeyCodes::MouseButton>> mouseItems
        = {{"LEFT", KeyCodes::MouseButton::LEFT},
           {"RIGHT", KeyCodes::MouseButton::RIGHT},
           {"MIDDLE", KeyCodes::MouseButton::MIDDLE}};
    lua_.new_enum<KeyCodes::MouseButton, false>("MouseButton", mouseItems);
}

void LuaManager::initWorkers(int num_workers)
{
    workers_.reserve(num_workers);

    // Each worker gets its own Lua state (no shared state between workers)
    for (int i = 0; i < num_workers; i++)
    {
        auto worker = std::make_unique<LuaWorker>(i);
        worker->init();
        workers_.push_back(std::move(worker));
    }
}

// Coroutine that executes a batch of scripts on a specific worker
coro::task<std::vector<CommandBuffer>> LuaManager::executeBatchAsync(
    int worker_idx, const std::vector<WorkItem>& batch, const InputSnapshot& input,
    double time) const
{
    // Schedule this coroutine onto the thread pool
    co_await thread_pool_->schedule();

    // Execute all scripts in this batch on the assigned worker
    auto results
        = workers_[worker_idx]->executeBatch(batch, input, query_cache_.get(), time, handle_map_);

    co_return results;
}

void LuaManager::update(float dt)
{
    if (not scene_) return;

    elapsed_time_ += dt;

    const auto input_snap = captureInput();
    query_cache_->clear();
    const auto scripts = scene_->getEntityManager().getRegistry().view<LuaScript>();
    std::vector<std::vector<WorkItem>> worker_batches(workers_.size());

    for (auto [entity_handle, script] : scripts.each())
    {
        if (auto entity = scene_->getEntityManager().getEntity(entity_handle);
            entity.hasComponent<ecs::LifecycleComponent>())
        {
            auto& lifecycle = entity.getComponent<ecs::LifecycleComponent>();
            if (!lifecycle.isAlive()) continue;
        }

        std::size_t const script_hash = static_cast<std::size_t>(entity_handle);
        std::size_t const worker_id = script_hash % workers_.size();

        worker_batches[worker_id].emplace_back(&script, dt);
    }

    std::vector<coro::task<std::vector<CommandBuffer>>> tasks;
    tasks.reserve(workers_.size());

    for (auto const i : std::views::iota(0u, workers_.size()))
    {
        tasks.push_back(executeBatchAsync(i, worker_batches[i], input_snap, elapsed_time_));
    }

    auto all_results = coro::sync_wait(coro::when_all(std::move(tasks)));

    std::vector<CommandBuffer> all_commands;

    for (auto& result : all_results)
    {
        auto worker_commands = result.return_value();
        all_commands.insert(all_commands.end(), std::make_move_iterator(worker_commands.begin()),
                            std::make_move_iterator(worker_commands.end()));
    }

    handle_map_.clear();

    for (auto const& cmd_buf : all_commands)
    {
        applyCommands(cmd_buf.commands, scene_, handle_map_);
    }
}

void LuaManager::onShutdown()
{
    for (auto& worker : workers_)
    {
        worker->shutdown();
    }
    workers_.clear();

    // Shutdown thread pool (waits for all tasks to complete)
    thread_pool_.reset();

    query_cache_.reset();
    scene_ = nullptr;
    handle_map_.clear();

    lua_.collect_gc();
    lua_ = sol::state();

    SDK_INFO("LuaManager shut down (libcoro)");
}

}  // namespace roen::lua
