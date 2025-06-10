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

#define CONFIG_FILE "./config"

namespace utils {

//  Config class retrieves configuration variables from CONFIG_FILE
//  It makes them accessible withing the code by sharing a singleton instance
class Config {
public:
    static Config* getConfig() {
        if (config_ == nullptr) {
            config_ = new Config;
        }
        return config_;
    }

    static std::string get(const std::string& key) {
        return configMap_[key];
    }

    static int getAsInt(const std::string& key) {
        return std::stoi(configMap_[key]);
    }

    static short getAsShort(const std::string& key) {
        short valueAsShort = std::stoi(configMap_[key]);
        if (valueAsShort > std::numeric_limits<short>::max() || valueAsShort < std::numeric_limits<short>::min())
            std::cerr << "Value not of short size!" << std::endl;
        return valueAsShort;
    }

    static long getAsLong(const std::string& key) {
        return std::stol(configMap_[key]);
    }

    static float getAsFloat(const std::string& key) {
        return std::stof(configMap_[key]);
    }

    static double getAsDouble(const std::string& key) {
        return std::stod(configMap_[key]);
    }

    static void set(const std::string& key, const std::string& value) {
        configMap_[key] = value;
    }

private:
    Config() {
        readConfig();
    };
    ~Config() = default;

    static void readConfig() {
        std::ifstream configFile(CONFIG_FILE);
        std::string line;

        if (configFile.is_open()) {
            while (std::getline(configFile, line)) {
                //  comment functionality
                if (line[0] == '#')
                    continue;

                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);
                    clean(key);
                    clean(value);
                    configMap_[key] = value;
                }
            }
            configFile.close();
        } else {
            std::cerr << "Error opening config file!" << std::endl;
        }
    }

    //  removes special characters and spaces from strings
    static void clean(std::string& stringToClean) {
        bool notInsideQuotes = true;
        stringToClean.erase(
            std::remove_if(stringToClean.begin(), stringToClean.end(),
                [&notInsideQuotes](char c) { 
                    static char charsToRemove[] = {
                        '\'', '\a', '\b', '\f', '\n', '\r', '\t', '\v', ' '};
                    for (auto charToRemove : charsToRemove) {
                        //  stops deletion of special characters if between qoutes
                        if (c == '\"') {
                            notInsideQuotes = notInsideQuotes? false : true;
                            return true;
                        }
                        if (c == charToRemove && notInsideQuotes)
                            return true;
                    }
                    return false;
                }
            ),
            stringToClean.end()
        );
    }

    static Config* config_;
    static std::map<std::string, std::string> configMap_;
};
using ConfigPtr = Config*;
using ConfigCPtr = const Config*;

} //  namespace utils

#endif //  #ifndef UTILS_CONFIG_HPP
