#ifndef UTILS_LOGGER_HPP
#define UTILS_LOGGER_HPP

#ifdef UTILS_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

enum ELogType {
    NONE,
    INFO,
    DEBUG,
    WARNING,
    ERROR
};

class Logger {
public:
    static Logger* getLogger(const std::string& name = "") {
        if (logger_ == nullptr) {
            logger_ = new Logger(name);
        }
        return logger_;
    }

    static void setDebugMode(const bool debugFlag) {
        debugFlag_ = debugFlag;
    }

    std::string getCurrentTimestamp() {
        const auto currentTime = toTimeT();
        char formatedTime[100];

        strftime(formatedTime, 100, "%d-%m-%Y %H:%M:%S", std::localtime(&currentTime));
        return formatedTime;
    }

    std::string log(ELogType level, const std::string& message, const std::string& channel = "") {
        if (level == DEBUG && debugFlag_ == false)
            return std::string();

        std::string logString;
        std::ofstream logFile(logName_, std::ios::app);

        if (logFile.is_open()) {
            logString += "[" + getCurrentTimestamp() + "] " ;
            if (!channel.empty())
                logString += "[" + channel + "] ";
            switch (level) {
                case INFO:
                    logString += "[INFO] ";
                    break;
                case DEBUG:
                    logString += "[DBG] ";
                    break;
                case WARNING:
                    logString += "[WRN] ";
                    break;
                case ERROR:
                    logString += "[ERR] ";
                    break;
                case NONE:
                default:
                    break;
            }
            logString += message;
            logFile << logString << std::endl;
            logFile.close();
        } else {
            std::cerr << "Error opening log file!" << std::endl;
        }
        return logString;
    }

private:
    Logger(const std::string& name = "") {
        std::time_t initialTime = toTimeT();
        char formatedTime[100];
        strftime(formatedTime, 100, "%d-%m-%Y_%H-%M-%S", std::localtime(&initialTime));

        logName_ = "logs/" + std::string(formatedTime);
        logName_ += name.empty()? ".txt" : "-" + name + ".txt";
    }
    ~Logger() = default;

    std::time_t toTimeT() {
        const auto now = std::chrono::system_clock::now();
        return std::chrono::system_clock::to_time_t(now);
    }

    std::string logName_;
    static Logger* logger_;
    static bool debugFlag_;
};

Logger* Logger::logger_ = nullptr;
bool Logger::debugFlag_ = false;

using LoggerPtr = Logger*;

#endif //  #ifndef UTILS_LOGGER_HPP
