#include "i18nManager/i18nManager.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>

namespace fs = std::filesystem;

const std::string LANG_DIR = "../i18n/";

json i18nManager::LoadLanguageFile(const std::string& lang_code) const
{
    std::string filename = LANG_DIR + lang_code + ".json";
    
    if (!fs::exists(filename)) {
        std::cerr << "ERRO I18N: Arquivo de idioma nao encontrado: " << filename << std::endl;
        return json::object();
    }

    std::ifstream i(filename);
    json j;
    try {
        i >> j;
    } catch (const std::exception& e) {
        std::cerr << "ERRO I18N: Falha ao parsear JSON de " << filename << ": " << e.what() << std::endl;
        return json::object();
    }
    return j;
}

std::map<std::string, json> i18nManager::LoadAllLanguages()
{
    std::map<std::string, json> languages;

    // Lógica simplificada: carregamos apenas os idiomas esperados.
    // Em produção, você deve iterar sobre LANG_DIR.
    languages["en_US"] = LoadLanguageFile("en_US");
    languages["pt_BR"] = LoadLanguageFile("pt_BR");
    
    if (languages.find(DEFAULT_LANGUAGE) == languages.end() || languages.at(DEFAULT_LANGUAGE).empty()) {
        std::cerr << "ERRO FATAL I18N: O idioma padrao (" << DEFAULT_LANGUAGE << ") nao pode ser carregado ou esta vazio." << std::endl;
    }

    return languages;
}

i18nManager::i18nManager()
{
    m_all_languages = LoadAllLanguages();
    SetLanguage(DEFAULT_LANGUAGE);
}

i18nManager &i18nManager::Get()
{
    static i18nManager instance;
    return instance;
}

void i18nManager::SetLanguage(const std::string& lang_code)
{
    auto it = m_all_languages.find(lang_code);
    
    if (it != m_all_languages.end() && !it->second.empty()) {
        m_current_language = &(it->second);
        m_current_lang_code = lang_code;
        std::cout << "Idioma definido para: " << lang_code << std::endl;
    } else {
        if (m_current_language == nullptr || lang_code != m_current_lang_code) {
             std::cerr << "AVISO I18N: Idioma '" << lang_code << "' nao encontrado. Usando padrao: " << m_current_lang_code << std::endl;
        }
    }
}

std::string i18nManager::GetString(const std::string& key) const
{
    if (!m_current_language) {
        return "[ERRO: NENHUM IDIOMA CARREGADO]";
    }

    std::istringstream ss(key);
    std::string segment;
    const json* current_node = m_current_language; 

    while (std::getline(ss, segment, '.')) {
        if (current_node->contains(segment)) {
            current_node = &((*current_node)[segment]);
        } else {
            if (m_current_lang_code != DEFAULT_LANGUAGE) {
                return i18nManager::Get().GetString(key); 
            }
            std::cerr << "AVISO I18N: Chave nao encontrada: " << key << " (" << m_current_lang_code << ")" << std::endl;
            return "!!" + key + "!!";
        }
    }

    if (current_node->is_string()) {
        return current_node->get<std::string>();
    } else {
        return "[ERRO: VALOR INVALIDO PARA CHAVE " + key + "]";
    }
}