#ifndef ROEN_LOG_LOGGER_HPP
#define ROEN_LOG_LOGGER_HPP

/*
 * Workaround for raylib symbol clash with Windows symbols imported in spdlog
 */
#if defined(_WIN32)
#    define NOGDI   // All GDI defines and routines
#    define NOUSER  // All USER defines and routines
#endif

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#if defined(_WIN32)  // raylib uses these names as function parameters
#    undef near
#    undef far
#endif
/*
 * End of Workaround
 */

#include <memory>

namespace roen::log
{

class Logger
{
public:
    static void Init();

    static void setSdkLogLevel(spdlog::level::level_enum level);
    static void setAppLogLevel(spdlog::level::level_enum level);
    static void setLuaLogLevel(spdlog::level::level_enum level);

    static std::shared_ptr<spdlog::logger>& getSdkLogger();
    static std::shared_ptr<spdlog::logger>& getAppLogger();
    static std::shared_ptr<spdlog::logger>& getLuaLogger();

private:
    inline static std::shared_ptr<spdlog::logger> sdkLogger_;
    inline static std::shared_ptr<spdlog::logger> appLogger_;
    inline static std::shared_ptr<spdlog::logger> luaLogger_;
};

}  // namespace roen::log

// Sdk log macros
#define LOG_LEVEL_INFO spdlog::level::info
#define LOG_LEVEL_OFF spdlog::level::off

#ifdef IS_DEBUG
#    define SDK_ERROR(...) roen::log::Logger::getSdkLogger()->error(__VA_ARGS__)
#    define SDK_INFO(...) roen::log::Logger::getSdkLogger()->info(__VA_ARGS__)
#    define SDK_WARN(...) roen::log::Logger::getSdkLogger()->warn(__VA_ARGS__)
#    define SDK_TRACE(...) roen::log::Logger::getSdkLogger()->trace(__VA_ARGS__)
#    define SDK_CRITICAL(...) roen::log::Logger::getSdkLogger()->critical(__VA_ARGS__)
#    define SET_SDK_LOG_LEVEL(...) roen::log::Logger::setSdkLogLevel(__VA_ARGS__)
#    define GET_SDK_LOG_LEVEL() roen::log::Logger::getSdkLogger()->level()

// App log macros
#    define APP_ERROR(...) roen::log::Logger::getAppLogger()->error(__VA_ARGS__)
#    define APP_INFO(...) roen::log::Logger::getAppLogger()->info(__VA_ARGS__)
#    define APP_WARN(...) roen::log::Logger::getAppLogger()->warn(__VA_ARGS__)
#    define APP_TRACE(...) roen::log::Logger::getAppLogger()->trace(__VA_ARGS__)
#    define APP_CRITICAL(...) roen::log::Logger::getAppLogger()->critical(__VA_ARGS__)
#    define SET_APP_LOG_LEVEL(...) roen::log::Logger::setAppLogLevel(__VA_ARGS__)
#    define GET_APP_LOG_LEVEL() roen::log::Logger::getAppLogger()->level()

// Lua log macros
#    define LUA_ERROR(...) roen::log::Logger::getLuaLogger()->error(__VA_ARGS__)
#    define LUA_INFO(...) roen::log::Logger::getLuaLogger()->info(__VA_ARGS__)
#    define LUA_WARN(...) roen::log::Logger::getLuaLogger()->warn(__VA_ARGS__)
#    define LUA_TRACE(...) roen::log::Logger::getLuaLogger()->trace(__VA_ARGS__)
#    define LUA_CRITICAL(...) roen::log::Logger::getLuaLogger()->critical(__VA_ARGS__)
#    define SET_LUA_LOG_LEVEL(...) roen::log::Logger::setLuaLogger(__VA_ARGS__)
#    define GET_LUA_LOG_LEVEL() roen::log::Logger::getLuaLogger()->level()

#else
#    define SDK_ERROR(...)
#    define SDK_INFO(...)
#    define SDK_WARN(...)
#    define SDK_TRACE(...)
#    define SDK_CRITICAL(...)
#    define SET_SDK_LOG_LEVEL(...)
#    define GET_SDK_LOG_LEVEL() LOG_LEVEL_INFO

// App log macros
#    define APP_ERROR(...)
#    define APP_INFO(...)
#    define APP_WARN(...)
#    define APP_TRACE(...)
#    define APP_CRITICAL(...)
#    define SET_APP_LOG_LEVEL(...)
#    define GET_APP_LOG_LEVEL() LOG_LEVEL_INFO

// Lua log macros
#    define LUA_ERROR(...)
#    define LUA_INFO(...)
#    define LUA_WARN(...)
#    define LUA_TRACE(...)
#    define LUA_CRITICAL(...)
#    define SET_LUA_LOG_LEVEL(...)
#    define GET_LUA_LOG_LEVEL() LOG_LEVEL_INFO
#endif  // IS_DEBUG

#endif  // ROEN_LOG_LOGGER_HPP