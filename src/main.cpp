#include <algorithm>
#include <iostream>
#include <dpp/dpp.h>
#include <dpp/message.h>
#include <dpp/nlohmann/json.hpp>
#include <string>

#include "./database/db.hpp"
#include "./handlers/events.hpp"
#include "./handlers/commands.hpp"

using namespace std;
using json = nlohmann::json;

int main() {
    json reader;
    {
        std::ifstream reading("../config.json");
        if (!reading) {
            std::cout << "No configuration file found! Please add it with the name: \"config.json\"!";
            std::cin.get();
            exit(0);
        }
        reading >> reader;
    }

    const auto token = reader["token"];
    string mongo_uri = reader["mongo_db_uri"];
    dbmain(mongo_uri);
    dpp::cluster bot(token, dpp::i_default_intents | dpp::i_message_content);
    bot.on_log(dpp::utility::cout_logger());

    bot.on_ready([&bot](const dpp::ready_t& event) {
        LoadEvents(bot);
        LoadCommands(bot);
    });

    bot.start(dpp::st_wait);
}