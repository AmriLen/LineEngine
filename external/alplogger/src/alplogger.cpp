#include "../include/alplogger/alplogger.h"
#include <iostream>
#include <fstream>
#include <ctime>

namespace LOGGER
{
    static std::ofstream logFile;

    void Init()
    {
        logFile.open("LineEngine.log", std::ios::out | std::ios::trunc);
        if (!logFile.is_open())
        {
            std::cerr << "[Logger] Failed to open log file!" << std::endl;
        }
    }

    void Shutdown()
    {
        if (logFile.is_open())
            logFile.close();
    }

    std::string GetTimestamp()
    {
        time_t now = time(nullptr);
        tm* localTime = localtime(&now);
        char buffer[32];
        strftime(buffer, sizeof(buffer), "%G-%m-%d %H:%M:%S", localTime);
        return std::string(buffer);
    }

    void Log(Level level, const std::string& message)
    {
        std::string prefix;
        std::string prefixname;
        switch (level)
        {
        case Level::INFO:       
            prefix = "[\033[32mINFO\033[0m] "; 
            prefixname = "[INFO] ";
            break;
        case Level::WARN:       
            prefix = "[\033[33mWARN\033[0m] ";
            prefixname = "[WARN] ";
            break;
        case Level::ERROR:      
            prefix = "[\033[31mERROR\033[0m] ";
            prefixname = "[ERROR] ";
            break;
        case Level::CRITICAL:   
            prefix = "[\033[37;41mCRITICAL\033[0m] ";
            prefixname = "[CRITICAL] ";
            break;
        }

        std::string timestamp = GetTimestamp();
        std::string finalMessage = "[" + timestamp + "] " + prefix + message;
        std::string finalFileMessage = "[" + timestamp + "] " + prefixname + message;

        std::cerr << finalMessage << std::endl;

        if (logFile.is_open())
            logFile << finalFileMessage << std::endl;
    }

    void INFO(const std::string& message) { Log(Level::INFO, message); }
    void WARN(const std::string& message) { Log(Level::WARN, message); }
    void ERROR(const std::string& message) { Log(Level::ERROR, message); }
    void CRITICAL(const std::string& message) { Log(Level::CRITICAL, message); }
}
