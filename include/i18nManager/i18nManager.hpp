#pragma once

#include <dpp/nlohmann/json.hpp>
#include <string>
#include <map>

using json = nlohmann::json;

/**
 * @brief Manages Internationalization (i18n) for the application.
 *
 * This class implements the Singleton pattern to provide global access to
 * loaded language resources (translations). It handles loading JSON files,
 * setting the current language, and retrieving translated strings.
 */
class i18nManager
{
public:
    /// The default fallback language code (e.g., "en_US").
    const std::string DEFAULT_LANGUAGE = "en_US";

    /**
     * @brief Gets the single instance of the i18nManager.
     * @return A reference to the i18nManager Singleton instance.
     */
    static i18nManager &Get();

    /**
     * @brief Sets the language to be used for translation.
     *
     * If the requested language code is not found, the manager reverts to 
     * the DEFAULT_LANGUAGE.
     * * @param lang_code The language code to set (e.g., "pt_BR").
     */
    void SetLanguage(const std::string& lang_code);

    /**
     * @brief Retrieves a translated string based on its key.
     *
     * It supports nested keys using dot notation (e.g., "messages.welcome_user").
     * If the key is not found in the current language, it attempts to fall back
     * to the DEFAULT_LANGUAGE before returning an error marker.
     *
     * @param key The translation key (e.g., "roosters.stats_health").
     * @return The translated string, or an error marker if the key is not found.
     */
    std::string GetString(const std::string& key) const;

private:
    /// Mapping storage for all loaded translations (language_code -> JSON object).
    std::map<std::string, json> m_all_languages; 
    
    /// Pointer to the JSON object of the currently selected language for fast lookup.
    const json* m_current_language = nullptr;
    
    /// The code of the currently active language.
    std::string m_current_lang_code;

    /**
     * @brief Private constructor to enforce Singleton pattern.
     *
     * Initializes the manager by loading all available language files.
     */
    i18nManager();
    
    /**
     * @brief Loads all language files from the defined directory (e.g., "./i18n/").
     * @return A map containing all successfully loaded languages and their JSON data.
     */
    std::map<std::string, json> LoadAllLanguages();

    /**
     * @brief Utility function to load a single language JSON file from disk.
     * @param lang_code The language code corresponding to the filename (e.g., "en_US").
     * @return The loaded JSON object for the specified language.
     */
    json LoadLanguageFile(const std::string& lang_code) const;

    // Delete copy constructor and assignment operator to prevent cloning (Singleton enforcement).
    i18nManager(const i18nManager&) = delete;
    i18nManager& operator=(const i18nManager&) = delete;
};