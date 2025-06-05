#ifndef ROEN_LUA_LUA_MANAGER_HPP
#define ROEN_LUA_LUA_MANAGER_HPP

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
    void update();

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
void LuaManager::callEventHandler(Args&&... args)
{
    luaEventManager_["handleEvents"](luaEventManager_, std::forward<Args>(args)...);
}

}  // namespace roen::lua

#endif  // ROEN_LUA_LUA_MANAGER_HPP