#include "utils/logger.hpp"

namespace utils {
//  to avoid multiple declaration linking error
LoggerPtr Logger::logger_ = nullptr;
std::string Logger::logName_;
bool Logger::debugFlag_ = false;


Logger::Logger(const std::string& name, const std::string& path) {
    std::time_t initialTime = toTimeT();
    char formatedTime[100];
    strftime(formatedTime, 100, "%d-%m-%Y_%H-%M-%S", std::localtime(&initialTime));

    std::system(std::string("mkdir -p " + path).c_str());
    logName_ = path + "/" + std::string(formatedTime);
    logName_ += name.empty()? ".txt" : "-" + name + ".txt";
}

LoggerPtr Logger::getLogger(const std::string& name) {
    if (logger_ == nullptr) {
        logger_ = new Logger(name);
    }
    return logger_;
}

void Logger::setDebugMode(const bool debugFlag) {
    debugFlag_ = debugFlag;
}

std::string Logger::getCurrentTimestamp() {
    const auto currentTime = toTimeT();
    char formatedTime[100];

    strftime(formatedTime, 100, "%d-%m-%Y %H:%M:%S", std::localtime(&currentTime));
    return formatedTime;
}

std::string Logger::log(ELogType level, const std::string& message, const std::string& channel) {
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

std::time_t Logger::toTimeT() {
    const auto now = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t(now);
}

} //  namsepace utils
