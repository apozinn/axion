#include <iostream>
#include <string>
#include <dpp/dpp.h>
#include "spdlog/spdlog.h"

using namespace std;
using namespace dpp;

void Ban(cluster& bot, const slashcommand_t& event, const guild& guild, user& author) {
	const auto targetID = get<snowflake>(event.get_parameter("user"));
	const auto target = guild.members.find(targetID);
	const auto reason = event.get_parameter("reason");

	if (target == guild.members.end()) {
		event.reply(
			dpp::message("🚫 **Não foi possivel encontrar este usuário.**").set_flags(dpp::m_ephemeral)
		);
		return;
	}

	if (!guild.base_permissions(event.command.member).has(dpp::p_manage_messages)) {
		event.reply(
			dpp::message("🚫 **Você não tem permissão para usar este comando.**").set_flags(dpp::m_ephemeral)
		);
		return;
	}

	if (!event.command.app_permissions.has(dpp::p_ban_members)) {
		event.reply(
			dpp::message("🚫 **Eu não tenho permissão para banir usuários.**").set_flags(dpp::m_ephemeral)
		);
		return;
	}

	if (targetID == guild.owner_id) {
		event.reply(
			dpp::message("🚫 **Você não pode banir o dono deste servidor.**").set_flags(dpp::m_ephemeral)
		);
		return;
	}

	if(targetID == author.id) {
		event.reply(
			dpp::message("🚫 **Você não pode banir você mesmo.**").set_flags(dpp::m_ephemeral)
		);
		return;
	}

	const auto button_confirm = dpp::component()
		.set_label("Confirmar")
		.set_type(dpp::cot_button)
		.set_style(dpp::cos_success)
		.set_id("ban_button_confirm");
	const auto button_cancel = dpp::component()
		.set_label("Cancelar")
		.set_type(dpp::cot_button)
		.set_style(dpp::cos_danger)
		.set_id("ban_button_cancel");

	std::string _reason = "Sem motivo";
	if(std::holds_alternative<std::string>(reason)) _reason = std::get<std::string>(reason);

	bot.on_button_click([&bot, author, targetID, guild, _reason](const button_click_t& btn) {
		if(!author.id == btn.command.usr.id) return false;

		if(btn.custom_id == "ban_button_confirm") {
			bot.set_audit_reason(_reason);
			bot.guild_ban_add(guild.id, targetID);
			btn.reply(
				dpp::interaction_response_type::ir_update_message,
				dpp::message(fmt::format("<@{}> foi banido com sucesso.", targetID))
			);
			return true;
		}
		if(btn.custom_id == "ban_button_cancel") {
			btn.reply(
				dpp::interaction_response_type::ir_update_message,
				dpp::message("Banimento cancelado.")
			);
			return true;
		}
		return false;
	});

	dpp::embed embed = dpp::embed()
		.set_color(colors::sti_blue)
		.set_description(fmt::format("*Você confirma o banimento de <@{}> por **{}**?*", targetID, _reason))
		.set_footer(dpp::embed_footer().set_text("ban").set_icon(author.get_avatar_url()))
		.set_timestamp(time(0));
	event.reply(
		dpp::message().add_embed(embed).add_component(
		dpp::component().add_component(button_confirm).add_component(button_cancel))
	);
	return;
}