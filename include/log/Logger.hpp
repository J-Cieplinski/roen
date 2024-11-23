#ifndef ROEN_LOG_LOGGER_HPP
#define ROEN_LOG_LOGGER_HPP

/*
 * Workaround for raylib symbol clash with Windows symbols imported in spdlog
 */
#if defined(_WIN32)
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#endif

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#if defined(_WIN32)           // raylib uses these names as function parameters
#undef near
#undef far
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

    static std::shared_ptr<spdlog::logger>& getSdkLogger();
    static std::shared_ptr<spdlog::logger>& getAppLogger();

private:
    inline static std::shared_ptr<spdlog::logger> sdkLogger_;
    inline static std::shared_ptr<spdlog::logger> appLogger_;
};

} //roen::log

//Sdk log macros
#ifdef IS_DEBUG
    #define SDK_ERROR(...)      roen::log::Logger::getSdkLogger()->error(__VA_ARGS__)
    #define SDK_INFO(...)       roen::log::Logger::getSdkLogger()->info(__VA_ARGS__)
    #define SDK_WARN(...)       roen::log::Logger::getSdkLogger()->warn(__VA_ARGS__)
    #define SDK_TRACE(...)      roen::log::Logger::getSdkLogger()->trace(__VA_ARGS__)
    #define SDK_CRITICAL(...)   roen::log::Logger::getSdkLogger()->critical(__VA_ARGS__)

    //App log macros
    #define APP_ERROR(...)      roen::log::Logger::getAppLogger()->error(__VA_ARGS__)
    #define APP_INFO(...)       roen::log::Logger::getAppLogger()->info(__VA_ARGS__)
    #define APP_WARN(...)       roen::log::Logger::getAppLogger()->warn(__VA_ARGS__)
    #define APP_TRACE(...)      roen::log::Logger::getAppLogger()->trace(__VA_ARGS__)
    #define APP_CRITICAL(...)   roen::log::Logger::getAppLogger()->critical(__VA_ARGS__)
#else
    #define SDK_ERROR(...)
    #define SDK_INFO(...)
    #define SDK_WARN(...)
    #define SDK_TRACE(...)
    #define SDK_CRITICAL(...)

    //App log macros
    #define APP_ERROR(...)
    #define APP_INFO(...)
    #define APP_WARN(...)
    #define APP_TRACE(...)
    #define APP_CRITICAL(...)
#endif //IS_DEBUG

#endif  //ROEN_LOG_LOGGER_HPP