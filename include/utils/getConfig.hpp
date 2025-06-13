#ifndef UTILS_CONFIG_HPP
#define UTILS_CONFIG_HPP

#ifdef UTILS_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#define CONFIG_FILE "./config/config"

namespace utils {

//  Config class retrieves configuration variables from CONFIG_FILE
//  It makes them accessible withing the code by sharing a singleton instance
class Config {
public:
    static Config* getConfig();
    static std::string get(const std::string& key);
    static int getAsInt(const std::string& key);
    static short getAsShort(const std::string& key);
    static long getAsLong(const std::string& key);
    static float getAsFloat(const std::string& key);
    static double getAsDouble(const std::string& key);
    static void set(const std::string& key, const std::string& value);

private:
    Config();
    ~Config() = default;

    static void readConfig();
    static void clean(std::string& stringToClean);

    static Config* config_;
    static std::map<std::string, std::string> configMap_;
};
using ConfigPtr = Config*;
using ConfigCPtr = const Config*;

} //  namespace utils

#endif //  #ifndef UTILS_CONFIG_HPP
