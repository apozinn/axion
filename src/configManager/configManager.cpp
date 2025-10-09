#include "configManager/configManager.hpp"
#include <fstream>
#include <mutex>

ConfigManager::ConfigManager() = default;
ConfigManager::~ConfigManager() = default;

ConfigManager& ConfigManager::Get() {
    static ConfigManager instance;
    return instance;
}

bool ConfigManager::loadConfig(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    try {
        file >> configJson_;
    } catch (...) {
        return false;
    }
    return true;
}

nlohmann::json ConfigManager::getConfig() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return configJson_;
}

template<typename T>
T ConfigManager::getValue(const std::string& key, const T& defaultValue) const {
    std::lock_guard<std::mutex> lock(mutex_);
    if (configJson_.contains(key)) {
        return configJson_.value(key, defaultValue);
    }
    return defaultValue;
}

template int ConfigManager::getValue<int>(const std::string&, const int&) const;
template std::string ConfigManager::getValue<std::string>(const std::string&, const std::string&) const;
template bool ConfigManager::getValue<bool>(const std::string&, const bool&) const;