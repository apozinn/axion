#include <iostream>
#include <string>
#include <dpp/dpp.h>
#include <vector>
#include <cctype>
#include <random>
#include <algorithm>

#include "./headers/rinha.hpp"
#include "../../database/db.hpp"
#include "../../utils/randoms.hpp"

using namespace std;

void GaloInfo(
	dpp::cluster &bot, 
	const dpp::slashcommand_t &event, 
	dpp::guild &guild, 
	dpp::user &author
) {
	auto client = mongo_access::instance().get_connection();
    auto user_collection = (*client)["axion"]["users"];
    auto db_user = user_collection.find_one(make_document(kvp("_id", to_string(author.id))));
    vector<Rooster> roosters;

    if(db_user) {
		auto db_roosters = db_user->view()["roosters"].get_array().value;
		if(db_roosters[0]) {
			for (auto ele : db_roosters) {
				Rooster _rt; 
				_rt.withProps(ele);
				roosters.push_back(_rt);
			}
			event.reply(dpp::message().add_component(dpp::component().add_component(generate_rt_menu(roosters, "select_galoinfo"))));

			bot.on_select_click([=, &bot](const dpp::select_click_t& slc) mutable {
				dpp::message message = slc.command.get_context_message();
				message.embeds.clear();
				message.components.clear();
				string value = slc.values.at(0);

				Rooster this_rt;
				for(auto rt : roosters) {
					if(rt.name == value) this_rt = rt;
				}

				if(slc.custom_id == "select_galoinfo") {
					auto embed = dpp::embed()
						.set_title(fmt::format("{} • Info", this_rt.name))
						.set_color(0xed0024)
						.add_field(
							"Propriedades:",
							fmt::format("Nome: **{}**\nLevel: **{}**\nClasse: **{}**\nRaridade: **{}**\nFraqueza: **{}**",
								this_rt.name,
								to_string(this_rt.level),
								this_rt.classe,
								this_rt.rarity,
								this_rt.weakness
							)
						)
						.add_field(
							"Status:",
							fmt::format("Vida: **{}**\nAtaque: **{}**\nDefesa: **{}**\nEstamina: **{}**\nVelocidade: **{}**\nMoral: **{}**",
								to_string(this_rt.life),
								to_string(this_rt.atk),
								to_string(this_rt.def),
								to_string(this_rt.stamina),
								to_string(this_rt.velocity),
								to_string(this_rt.moral)
							)
						)
						.set_footer(dpp::embed_footer().set_text(author.username).set_icon(author.get_avatar_url()))
						.set_timestamp(time(0));
					message.add_embed(embed);
					bot.message_edit(message);
				}
			});
		} else {
			event.reply(dpp::message("Você nao possui galos."));	
		}
	} else {
		event.reply(dpp::message("Você ainda nao possui galos."));
	}
}