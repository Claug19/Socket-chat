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

namespace utils {

//  Logger class is a general purpose logger
//  Suports debug mode
class Logger {
public:
    static Logger* getLogger(const std::string& name = "");
    static void setDebugMode(const bool debugFlag);
    static std::string getCurrentTimestamp();
    static std::string log(ELogType level, const std::string& message, const std::string& channel = "");

private:
    Logger(const std::string& name = "", const std::string& path = "logs");
    ~Logger() = default;

    static std::time_t toTimeT();

    static Logger* logger_;
    static std::string logName_;
    static bool debugFlag_;
};
using LoggerPtr = Logger*;
using LoggerCPtr = const Logger*;

} //  namespace utils

#endif //  #ifndef UTILS_LOGGER_HPP
