#include <iostream>
#include <string>
#include <dpp/dpp.h>
#include "spdlog/spdlog.h"

using namespace std;
using namespace dpp;

void ServerInfo(cluster& bot, const slashcommand_t& event, const guild& guild, user& author) {
	dpp::embed embed = dpp::embed()
		.set_author(guild.name, "", guild.get_icon_url())
		.set_color(colors::sti_blue)
		.set_thumbnail(guild.get_icon_url())
		.add_field("Nome:", fmt::format("*{}*\n*({})*", guild.name, guild.id))
		.add_field("Posse:", fmt::format("<@{}>\n*({})*", guild.owner_id, guild.owner_id))
		.add_field("Criação", to_string(guild.get_creation_time()))
		.add_field("Membros:", to_string(guild.member_count))
		.add_field("Canais:", to_string(guild.channels.size()))
		.add_field("Cargos:", to_string(guild.roles.size()))
		.set_footer(dpp::embed_footer().set_text(author.username).set_icon(author.get_avatar_url()))
		.set_timestamp(time(0));
	event.reply(dpp::message().add_embed(embed));
	return;
}