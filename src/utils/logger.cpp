#include "logger.hpp"

namespace utils {
//  to avoid multiple linking error
Logger* Logger::logger_ = nullptr;
std::string Logger::logName_;
bool Logger::debugFlag_ = false;

} //  namsepace utils
