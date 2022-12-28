#pragma once

#include <iostream>
#include <string>
#include <dpp/dpp.h>
#include <map>
#include <functional>

inline void LoadCommands(dpp::cluster& bot) {
	bot.global_command_create(
		dpp::slashcommand("ping", "Ping pong!", bot.me.id)
	);

	bot.global_command_create(
		dpp::slashcommand("servericon", "Get server icon", bot.me.id)
	);

	bot.global_command_create(
		dpp::slashcommand("serverinfo", "Get server infos", bot.me.id)
	);

	dpp::slashcommand profileCommand("profile", "Get user profile", bot.me.id);
	profileCommand.add_option(dpp::command_option(dpp::co_user, "user", "User to get profile.", true));
	bot.global_command_create(profileCommand);

	dpp::slashcommand avatarCommand("avatar", "Get user avatar", bot.me.id);
	avatarCommand.add_option(dpp::command_option(dpp::co_user, "user", "User to get avatar.", true));
	bot.global_command_create(avatarCommand);

	dpp::slashcommand userinfoCommand("userinfo", "Get user info", bot.me.id);
	userinfoCommand.add_option(dpp::command_option(dpp::co_user, "user", "User to get info.", true));
	bot.global_command_create(userinfoCommand);
	
	dpp::slashcommand purgeCommand("purge", "Delete yours message", bot.me.id);
	purgeCommand.add_option(dpp::command_option(dpp::co_number, "count", "Count of messages", true).set_min_value(2).set_max_value(100));
	bot.global_command_create(purgeCommand);

	dpp::slashcommand clearCommand("clear", "Clear message of the chat", bot.me.id);
	clearCommand.add_option(dpp::command_option(dpp::co_number, "count", "Count of messages", true).set_min_value(2).set_max_value(100));
	bot.global_command_create(clearCommand);

	dpp::slashcommand banCommand("ban", "Ban a user", bot.me.id);
	banCommand.add_option(dpp::command_option(dpp::co_user, "user", "User will be banned", true));
	banCommand.add_option(dpp::command_option(dpp::co_string, "reason", "Reason of the ban", false));
	bot.global_command_create(banCommand);

	return;
}