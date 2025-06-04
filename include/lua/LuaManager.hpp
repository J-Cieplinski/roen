#ifndef ROEN_LUA_LUA_MANAGER_HPP
#define ROEN_LUA_LUA_MANAGER_HPP

#include <lua/LuaCallable.hpp>

#include <lua/LuaScript.hpp>

#include <memory>

#include <sol2/sol.hpp>

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
    LuaManager operator=(LuaManager&) = delete;
    LuaManager operator=(LuaManager&&) = delete;
    LuaManager(LuaManager&&) = delete;

    ~LuaManager();

    sol::state_view getState() const;

    void onInit(interfaces::Scene* scene);
    void onShutdown();

    template <typename... Args>
    void registerUpdateable(std::string_view className, std::string_view fnName,
                            const std::filesystem::path& script, Args&&... args);

    void update();

    template <typename... Args>
    void update(Args&&... args);

    template <typename... Args>
    void callEventHandler(Args&&... args);

private:
    LuaManager() = default;

    static void InitLua();
    static void InitLuaApplication();
    static void InitLuaAssets();
    static void InitLuaInput();
    static void InitLuaLog();
    static void InitEventTypes();
    static void InitECS();
    static void InitScene();
    static void InitMathTypes();
    static void InitLuaEventHandler();
    static void InitUtils();

    inline static std::unique_ptr<LuaManager> instance_;
    sol::state lua_;
    std::vector<LuaCallable> updateables_;
    sol::table luaEventManager_;
    interfaces::Scene* scene_;
};

}  // namespace roen::lua

/*
 * Template implementation
 */

namespace roen::lua
{

template <typename... Args>
void LuaManager::registerUpdateable(std::string_view className, std::string_view fnName,
                                    const std::filesystem::path& script, Args&&... args)
{
    updateables_.emplace_back(lua_, className, fnName, script, std::forward<Args>(args)...);
}

template <typename... Args>
void LuaManager::update(Args&&... args)
{
    for (auto& update : updateables_)
    {
        update(std::forward<Args>(args)...);
    }

    auto scripts = scene_->getEntityManager().getRegistry().group<LuaScript>();
    for (const auto& [entity, script] : scripts.each())
    {
        script.onUpdate(1.f);
    }
}

template <typename... Args>
void LuaManager::callEventHandler(Args&&... args)
{
    luaEventManager_["handleEvents"](luaEventManager_, std::forward<Args>(args)...);
}

}  // namespace roen::lua

#endif  // ROEN_LUA_LUA_MANAGER_HPP