#include <lua/LuaScript.hpp>

#include <lua/LuaManager.hpp>

namespace roen::lua
{

LuaScript::LuaScript(ecs::Entity* entity)
    : entity_{entity}
{
}

void LuaScript::loadScript(const std::filesystem::path& filename)
{
    if (not std::filesystem::exists(filename))
    {
        SDK_ERROR("Failed to find script file {0}", filename.string().c_str());
        return;
    }

    auto state = LuaManager::Instance().getState();

    env_ = sol::environment(state, sol::create, state.globals());
    auto loadScriptRes = state.script_file(filename, env_, sol::script_pass_on_error);
    if (not loadScriptRes.valid())
    {
        const sol::error err = loadScriptRes;
        SDK_ERROR("Failed to load script {0}", filename.string().c_str());
        SDK_ERROR("Error: {0}", err.what());
        return;
    }

    env_["LuaComponent"] = this;
    onInit_ = std::make_unique<sol::protected_function>(env_["onInit"]);
    if (not onInit_->valid())
    {
        onInit_->reset();
    }
    auto result = onInit_->call();
    if (not result.valid())
    {
        const sol::error err = result;
        SDK_ERROR("Failed to execute onInit on script {0}", filename.string().c_str());
        SDK_ERROR("Error: {0}", err.what());
        return;
    }

    onUpdate_ = std::make_unique<sol::protected_function>(env_["onUpdate"]);
    if (not onUpdate_->valid())
    {
        onUpdate_.reset();
    }
}

void LuaScript::onUpdate(float dt) const
{
    if (onUpdate_)
    {
        auto result = onUpdate_->call(dt);
        if (not result.valid())
        {
            const sol::error err = result;
            SDK_ERROR("Failed to execute onInit");
            SDK_ERROR("Error: {0}", err.what());
        }
    }
}

ecs::Entity* LuaScript::getEntity() const
{
    return entity_;
}

}  // namespace roen::lua
