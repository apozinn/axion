#include <iostream>
#include "spdlog/spdlog.h"
#include <dpp/dpp.h>

using namespace std;
using namespace dpp;

void Ping(dpp::cluster& bot, const dpp::slashcommand_t& event, dpp::guild& guild, user& author) {
	const auto ws = bot.get_shard(0)->websocket_ping;
    const auto avatar = author.get_avatar_url();

	dpp::embed embed = dpp::embed()
		.set_color(dpp::colors::sti_blue)
		.set_description(fmt::format("🏓  | Meu ping no moment é de `{0:.02f} ms`", (bot.rest_ping + ws) * 1000))
		.set_footer(dpp::embed_footer().set_text(author.username).set_icon(avatar))
		.set_timestamp(time(0));
	dpp::message msg;
	msg.add_embed(embed);
	event.reply(msg);
	return;
}