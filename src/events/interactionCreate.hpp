#pragma once

#include <iostream>
#include <dpp/dpp.h>
#include <string>

//client commands
#include "../commands/client/ping.cpp"

//user commands
#include "../commands/user/profile.cpp"
#include "../commands/user/avatar.cpp"
#include "../commands/user/userinfo.cpp"
#include "../commands/user/purge.cpp"

//guild commands
#include "../commands/guild/clear.cpp"
#include "../commands/guild/servericon.cpp"
#include "../commands/guild/serverinfo.cpp"

//moderation commands 
#include "../commands/moderation/ban.cpp"

using namespace std;
using namespace dpp;

void interactionCreate(dpp::cluster& bot) {
	bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        dpp::guild guild = event.command.get_guild();
        dpp::user author = event.command.get_issuing_user();

        if (event.command.get_command_name() == "ping") {
            Ping(bot, event, guild, author);
        }

        if (event.command.get_command_name() == "profile") {
            Profile(bot, event, guild, author);
        }

        if (event.command.get_command_name() == "avatar") {
            Avatar(bot, event, guild, author);
        }

        if (event.command.get_command_name() == "userinfo") {
            UserInfo(bot, event, guild, author);
        }

        if (event.command.get_command_name() == "purge") {
            Purge(bot, event, guild, author);
        }

        if (event.command.get_command_name() == "clear") {
            Clear(bot, event, guild, author);
        }

        if (event.command.get_command_name() == "servericon") {
            ServerIcon(bot, event, guild, author);
        }

        if (event.command.get_command_name() == "serverinfo") {
            ServerInfo(bot, event, guild, author);
        }

        if (event.command.get_command_name() == "ban") {
            Ban(bot, event, guild, author);
        }
    });
	return;
}