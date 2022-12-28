#include <iostream>
#include <string>
#include <dpp/dpp.h>
#include "spdlog/spdlog.h"

using namespace std;
using namespace dpp;

void ServerIcon(cluster& bot, const slashcommand_t& event, const guild& guild, user& author) {
	dpp::embed embed = dpp::embed()
        .set_color(colors::sti_blue)
        .set_description(fmt::format("Icon de {}", guild.name))
        .set_image(guild.get_icon_url(1024))
        .set_footer(embed_footer().set_text(author.username).set_icon(author.get_avatar_url()))
        .set_timestamp(time(0));
    event.reply(dpp::message().add_embed(embed));
	return;
}