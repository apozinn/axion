#pragma once

#include <iostream>
#include <string>
#include <dpp/dpp.h>

#include "../commands/rinha/rinha.cpp"
#include "../commands/rinha/galoinfo.cpp"
#include "../commands/rinha/rank.cpp"
#include "../commands/rinha/shop.cpp"

using namespace std;

void interactionCreate(dpp::cluster& bot) {
	bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        dpp::guild guild = event.command.get_guild();
        dpp::user author = event.command.get_issuing_user();

        if (event.command.get_command_name() == "rinha") Rinha(bot, event, guild, author);
        if (event.command.get_command_name() == "galoinfo") GaloInfo(bot, event, guild, author);
        if (event.command.get_command_name() == "rank") Rank(bot, event, guild, author);
        if (event.command.get_command_name() == "shop") Shop(bot, event, guild, author);
    });
}