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

    void Init(); // ����������� � ��� ������������� ���-�����
    void Shutdown(); // ������� ���-���� ��� ����������

    void Log(Level level, const std::string& message);

    // ������� ����������
    void INFO(const std::string& message);
    void WARN(const std::string& message);
    void ERROR(const std::string& message);
    void CRITICAL(const std::string& message);
}
