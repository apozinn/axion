#include <iostream>
#include <dpp/dpp.h>
#include <string>
#include "spdlog/spdlog.h"

using namespace std;
using namespace dpp;

void Purge(cluster& bot, const slashcommand_t& event, const guild& guild, user& author) {
	const auto channelId = event.command.channel_id;
	const auto count = (int)round(std::get<double>(event.get_parameter("count")));
	
	dpp::message msg_error("There was an error in delete the messages.");
	msg_error.set_flags(m_ephemeral);

	try {
		bot.messages_get(channelId, 0, 0, 0, 100, [&](const confirmation_callback_t& callback) {
			if(!callback.is_error()) {
				dpp::message_map messages = std::get<dpp::message_map>(callback.value);
				std::vector<dpp::snowflake> msgIDS;

				for(auto& msg : messages) {
					if(msg.second.author.id == author.id) {
						msgIDS.push_back(msg.first);
					}
				}

				if(msgIDS.size() > 2) {
					bot.message_delete_bulk(msgIDS, event.command.channel_id, [&](const confirmation_callback_t& result) {
						if(result.is_error()) {
	                        bot.log(ll_error, result.get_error().message);
	                        event.reply(msg_error);
	                        return;
	                    }
	                });
				}
			}
		});
	} catch(...) {
		event.reply(msg_error);
		return;
	}

	dpp::message msg(fmt::format("{} Messages deleted successfully.", count));
	msg.set_flags(m_ephemeral);
	event.reply(msg);
	return;
}