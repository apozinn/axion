#include <string>
#include <iostream>
#include "configManager/configManager.hpp"
#include "client/client.hpp"
#include "i18nManager/i18nManager.hpp"
#include "databaseManager/databaseManager.hpp"

int main()
{
    ConfigManager::Get().loadConfig("../config.json");
    i18nManager::Get().SetLanguage(ConfigManager::Get().getValue<std::string>("language"));
    DatabaseManager::Get().Initialize();
    Client client;
    client.Connect();

    return 0;
}