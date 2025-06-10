#include "getConfig.hpp"

namespace utils {
//  to avoid multiple linking error
Config* Config::config_ = nullptr;
std::map<std::string, std::string> Config::configMap_;

} //  namsepace utils
