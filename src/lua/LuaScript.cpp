#include <../../include/lua/LuaManager.hpp>
#include <../../include/lua/LuaScript.hpp>

#include <filesystem>
#include <log/Logger.hpp>

namespace roen::lua
{

LuaScript::LuaScript(ecs::Entity entity)
    : entity_{entity}
{
}

void LuaScript::loadScript(std::string filename)
{
    std::filesystem::path const file{std::move(filename)};
    if (not std::filesystem::exists(file))
    {
        SDK_ERROR("Failed to find script file {0}", filename.c_str());
        return;
    }
    script_path_ = std::move(file);
}

void LuaScript::initializeScript(sol::state_view& lua)
{
    if (initialized_ or not deferred_script_path_.empty()) return;

    env_ = sol::environment(lua, sol::create, lua.globals());

    auto loadScriptRes = lua.script_file(script_path_, env_, sol::script_pass_on_error);
    if (not loadScriptRes.valid())
    {
        const sol::error err = loadScriptRes;
        SDK_ERROR("Failed to load script {0}: {1}", script_path_.string(), err.what());
        return;
    }

    onInit_ = std::make_unique<sol::protected_function>(env_["onInit"]);
    onUpdate_ = std::make_unique<sol::protected_function>(env_["onUpdate"]);
    onDeath_ = std::make_unique<sol::protected_function>(env_["onDeath"]);

    if (onInit_ && onInit_->valid())
    {
        CommandBuffer init_cmds;
        env_["_cmd_buf"] = &init_cmds;
        env_["self_entity"] = static_cast<entt::entity>(entity_);

        auto result = onInit_->call();
        if (not result.valid())
        {
            const sol::error err = result;
            SDK_ERROR("Failed to execute onInit: {0}", err.what());
        }
        pending_init_commands_ = std::move(init_cmds);
    }

    if (not onUpdate_ or not onUpdate_->valid())
    {
        onUpdate_.reset();
    }

    if (not onDeath_ or not onDeath_->valid())
    {
        onDeath_.reset();
    }

    initialized_ = true;
}

void LuaScript::loadScriptDeferred(std::string filename)
{
    deferred_script_path_ = std::move(filename);
}

void LuaScript::onUpdate(float dt, const InputSnapshot& input, const SceneQuery& query,
                         CommandBuffer* cmds, double time,
                         const std::unordered_map<Handle, entt::entity>&)
{
    if (not deferred_script_path_.empty())
    {
        loadScript(deferred_script_path_);
        deferred_script_path_.clear();
    }

    if (pending_init_commands_)
    {
        *cmds = std::move(*pending_init_commands_);
        pending_init_commands_ = std::nullopt;
    }

    if (not onUpdate_) return;

    env_["_cmd_buf"] = cmds;
    env_["self_entity"] = static_cast<entt::entity>(entity_);

    auto result = onUpdate_->call(dt, input, query, time);
    if (not result.valid())
    {
        const sol::error err = result;
        SDK_ERROR("Failed to execute onUpdate: {0}", err.what());
    }
}

float LuaScript::onDeath(const SceneQuery& query, CommandBuffer* cmds, double time)
{
    if (not onDeath_)
    {
        return 0.0f;  // Instant death if no onDeath function
    }

    env_["_cmd_buf"] = cmds;
    env_["self_entity"] = static_cast<entt::entity>(entity_);

    auto result = onDeath_->call(query, time);
    if (not result.valid())
    {
        const sol::error err = result;
        SDK_ERROR("Failed to execute onDeath: {0}", err.what());
        return 0.0f;
    }

    // onDeath can return death duration (in seconds)
    if (result.return_count() > 0)
    {
        sol::object ret = result;
        if (ret.is<float>())
        {
            return ret.as<float>();
        }
    }

    return 0.0f;  // Default: instant death
}

ecs::Entity LuaScript::getEntity() const
{
    return entity_;
}

bool LuaScript::isInitialized() const
{
    return initialized_;
}

}  // namespace roen::lua
