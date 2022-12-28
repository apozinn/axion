#include <iostream>
#include <string>
#include <dpp/dpp.h>
#include <ctime>
#include "spdlog/spdlog.h"

using namespace std;
using namespace dpp;

void UserInfo(cluster& bot, const slashcommand_t& event, guild& guild, user& author) {
	const auto user = get<snowflake>(event.get_parameter("user"));
	const auto target = find_user(user);
	const auto guildMember = bot.guild_get_member_sync(guild.id, user);

	const auto createdAt = to_string(target->get_creation_time());
	const auto joinedAt = to_string(guildMember.joined_at);

	dpp::embed embed = dpp::embed()
		.set_color(colors::sti_blue)
		.set_thumbnail(target->get_avatar_url())
		.add_field(
			"👤 UserName:",
			target->username
		)
		.add_field(
			"🔖 ID do usuario:",
			to_string(target->id)
        )
        .add_field(
        	"⏱ Conta criada em:",
        	createdAt
        )
        .add_field(
        	"⌛ Entrou em:",
        	joinedAt
        )
		.set_footer(dpp::embed_footer().set_text(author.username).set_icon(author.get_avatar_url()))
		.set_timestamp(time(0));
	message msg;
	msg.add_embed(embed);
	event.reply(msg); 
	return;
}