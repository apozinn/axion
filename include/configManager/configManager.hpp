#pragma once
#include <string>
#include <dpp/nlohmann/json.hpp>
#include <mutex>

/**
 * @brief Singleton class for managing configuration files.
 *
 * Provides thread-safe loading and access to configuration values from a JSON file.
 */
class ConfigManager {
public:
    /**
     * @brief Get the singleton instance of ConfigManager.
     * @return Reference to the ConfigManager instance.
     */
    static ConfigManager& Get();

    /**
     * @brief Load configuration from a JSON file.
     * @param filename Path to the configuration file.
     * @return True if loading was successful, false otherwise.
     */
    bool loadConfig(const std::string& filename);

    /**
     * @brief Get the entire configuration as a JSON object.
     * @return The loaded configuration as nlohmann::json.
     */
    nlohmann::json getConfig() const;

    /**
     * @brief Get a value from the configuration by key.
     * @tparam T Type of the value to retrieve.
     * @param key Key to look up in the configuration.
     * @param defaultValue Value to return if the key does not exist.
     * @return The value associated with the key, or defaultValue if not found.
     */
    template<typename T>
    T getValue(const std::string& key, const T& defaultValue = T()) const;

private:
    ConfigManager();
    ~ConfigManager();
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    mutable std::mutex mutex_;
    nlohmann::json configJson_;
};