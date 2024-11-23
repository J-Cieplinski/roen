#include <log/Logger.hpp>

#include <spdlog/sinks/stdout_color_sinks.h>

namespace roen::log
{

void Logger::Init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    sdkLogger_ = spdlog::stdout_color_mt("SDK");
    sdkLogger_->set_level(spdlog::level::trace);

    appLogger_ = spdlog::stderr_color_mt("APP");
    appLogger_->set_level(spdlog::level::trace);
}

void Logger::setSdkLogLevel(spdlog::level::level_enum level)
{
    if(!sdkLogger_)
    {
        throw std::runtime_error("SDK LOGGER was not initialized");
    }
    sdkLogger_->set_level(level);
}

void Logger::setAppLogLevel(spdlog::level::level_enum level)
{
    if(!appLogger_)
    {
        throw std::runtime_error("APP LOGGER was not initialized");
    }
    appLogger_->set_level(level);
}

std::shared_ptr<spdlog::logger>& Logger::getSdkLogger()
{
    if(!sdkLogger_)
    {
        throw std::runtime_error("SDK LOGGER was not initialized");
    }
    return sdkLogger_;
};

std::shared_ptr<spdlog::logger>& Logger::getAppLogger()
{
    if(!appLogger_)
    {
        throw std::runtime_error("APP LOGGER was not initialized");
    }
    return appLogger_;
};

} //roen::log