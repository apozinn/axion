#include <iostream>
#include <string>
#include "spdlog/spdlog.h"
#include <dpp/dpp.h>

using namespace std;
using namespace dpp;

void Avatar(cluster& bot, const slashcommand_t& event, const guild& guild, user& author) {
	const auto user = get<snowflake>(event.get_parameter("user"));
	const auto target = find_user(user);

	dpp::embed embed = dpp::embed()
        .set_color(colors::sti_blue)
        .set_description(fmt::format("Avatar de {}", target->username))
        .set_image(target->get_avatar_url(1024))
        .set_footer(embed_footer().set_text(author.username).set_icon(author.get_avatar_url()))
        .set_timestamp(time(0));
    message msg;
    msg.add_embed(embed);
    event.reply(msg);
	return;
}