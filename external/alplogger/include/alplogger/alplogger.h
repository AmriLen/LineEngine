#pragma once
#include <string>

namespace LOGGER
{
    enum class Level
    {
        INFO,
        WARN,
        ERROR,
        CRITICAL
    };

    void Init(); // опционально Ч дл€ инициализации лог-файла
    void Shutdown(); // закрыть лог-файл при завершении

    void Log(Level level, const std::string& message);

    // ”добные сокращени€
    void INFO(const std::string& message);
    void WARN(const std::string& message);
    void ERROR(const std::string& message);
    void CRITICAL(const std::string& message);
}
