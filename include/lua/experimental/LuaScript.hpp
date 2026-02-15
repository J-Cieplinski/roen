#ifndef ROEN_LUA_LUA_SCRIPT_HPP
#define ROEN_LUA_LUA_SCRIPT_HPP

#include <ecs/Entity.hpp>
#include <lua/experimental/CommandSystem.hpp>
#include <lua/experimental/ReadOnlyECS.hpp>

#include <memory>
#include <sol2/sol.hpp>
#include <string>

namespace roen::lua::experimental
{

class LuaScript
{
public:
    explicit LuaScript(ecs::Entity entity);

    void loadScript(std::string filename);
    void loadScriptDeferred(std::string filename);

    void onUpdate(float dt, const InputSnapshot& input, const SceneQuery& query,
                  CommandBuffer* cmds, double time,
                  const std::unordered_map<Handle, entt::entity>& handle_map);
    float onDeath(const SceneQuery& query, CommandBuffer* cmds, double time);

    ecs::Entity getEntity() const;
    bool isInitialized() const;
    void initializeScript(sol::state_view& lua);

    sol::environment env_;

private:
    std::unique_ptr<sol::protected_function> onInit_;
    std::unique_ptr<sol::protected_function> onUpdate_;
    std::unique_ptr<sol::protected_function> onDeath_;
    ecs::Entity entity_;
    std::string deferred_script_path_;
    std::optional<CommandBuffer> pending_init_commands_;
    std::filesystem::path script_path_;
    bool initialized_{false};
};

}  // namespace roen::lua::experimental

#endif
