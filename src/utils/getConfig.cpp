#include "utils/getConfig.hpp"

namespace utils {
//  to avoid multiple declaration linking error
ConfigPtr Config::config_ = nullptr;
std::map<std::string, std::string> Config::configMap_;

Config::Config() {
    readConfig();
};

ConfigPtr Config::getConfig() {
    if (config_ == nullptr) {
        config_ = new Config;
    }
    return config_;
}

std::string Config::get(const std::string& key) {
    return configMap_[key];
}

int Config::getAsInt(const std::string& key) {
    return std::stoi(configMap_[key]);
}

short Config::getAsShort(const std::string& key) {
    short valueAsShort = std::stoi(configMap_[key]);
    if (valueAsShort > std::numeric_limits<short>::max() || valueAsShort < std::numeric_limits<short>::min())
        std::cerr << "Value not of short size!" << std::endl;
    return valueAsShort;
}

long Config::getAsLong(const std::string& key) {
    return std::stol(configMap_[key]);
}

float Config::getAsFloat(const std::string& key) {
    return std::stof(configMap_[key]);
}

double Config::getAsDouble(const std::string& key) {
    return std::stod(configMap_[key]);
}

void Config::set(const std::string& key, const std::string& value) {
    configMap_[key] = value;
}

void Config::readConfig() {
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
void Config::clean(std::string& stringToClean) {
    bool notInsideQuotes = true;
    stringToClean.erase(
        std::remove_if(stringToClean.begin(), stringToClean.end(),
            [&notInsideQuotes](char c) {
                char charsToRemove[] = {
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

} //  namsepace utils
