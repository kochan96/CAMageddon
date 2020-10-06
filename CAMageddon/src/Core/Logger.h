#pragma once
#include <memory>
#include <spdlog/spdlog.h>

namespace CAMageddon
{
    class Logger
    {
    public:
        static void Init();
        inline static std::shared_ptr<spdlog::logger>& getAppLogger() { return m_appLogger; }

    private:
        static std::shared_ptr<spdlog::logger> m_appLogger;
    };


#define LOG_TRACE(...)     CAMageddon::Logger::getAppLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)      CAMageddon::Logger::getAppLogger()->info(__VA_ARGS__)
#define LOG_WARNING(...)   CAMageddon::Logger::getAppLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)     CAMageddon::Logger::getAppLogger()->error(__VA_ARGS__)
#define LOG_CRITITCAL(...) CAMageddon::Logger::getAppLogger()->critical(__VA_ARGS__)

#define LOG_ASSERT(x,...) if(!x){ LOG_ERROR(__VA_ARGS__); __debugbreak;}

}