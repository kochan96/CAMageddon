#include "Logger.h"
#include <spdlog/fmt/ostr.h>

namespace CAMageddon
{
    std::shared_ptr<spdlog::logger> Logger::m_appLogger;

    void Logger::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        m_appLogger = spdlog::stdout_color_mt("LOG");
        m_appLogger->set_level(spdlog::level::trace);
    }
}