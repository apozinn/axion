#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <random>
#include <algorithm>
#include "spdlog/spdlog.h"
#include <dpp/dpp.h>

#include "./headers/rinha.hpp"
#include "../../database/db.hpp"
#include "../../utils/randoms.hpp"

using namespace std;

void Rinha(
	dpp::cluster &bot, 
	const dpp::slashcommand_t &event, 
	dpp::guild &guild, 
	dpp::user &author
) {
	auto client = mongo_access::instance().get_connection();
	auto user_collection = (*client)["axion"]["users"];
	auto db_user = user_collection.find_one(make_document(kvp("_id", to_string(author.id))));

	vector<Rooster> roosters;
	string selected_rooster;

	auto add_roosters_to_user = [&](int length) {
		bsoncxx::document::value doc = make_document(
			kvp("roosters", [=](sub_array child) {
		    	for (int i = 0; i < length; ++i) {
			    	Rooster rt; 
					rt.create();
			        child.append(rooster_db_struct(rt));
		    	}
			})
		);
		return doc.view()["roosters"].get_array().value;
	};

	auto remove_roosters = [&](vector<Rooster> v_r, string excluded_rt) {
		bsoncxx::document::value doc = make_document(
			kvp("roosters", [=](sub_array child) {
		    	for (auto rt : v_r) {
		    		if(rt.name != excluded_rt) {
			        	child.append(rooster_db_struct(rt));
		    		}
		    	}
			})
		);
		return doc.view()["roosters"].get_array().value;
	};

	if(db_user) {
		auto db_roosters = db_user->view()["roosters"].get_array().value;

		if(!db_roosters[0]) {
			auto new_rts = add_roosters_to_user(3);
			for (auto ele : new_rts) {
				Rooster _rt; 
				_rt.withProps(ele);
				roosters.push_back(_rt);	
			}
			user_collection.update_one(
				make_document(kvp("_id", to_string(author.id))),
				make_document(kvp("$set", make_document(kvp("roosters", new_rts))))
		    );
		} else {
			for (auto ele : db_roosters) {
				Rooster _rt; 
				_rt.withProps(ele);
				roosters.push_back(_rt);
			}
		}
	} else {
		auto user_roosters = add_roosters_to_user(3);
		for (auto ele : user_roosters) {
			Rooster _rt; 
			_rt.withProps(ele);
			roosters.push_back(_rt);	
		}
	    user_collection.insert_one(make_document(
	    	kvp("_id", to_string(author.id)),
	    	kvp("golden_spike", 0),
	    	kvp("corns", 0),
	    	kvp("xp", 0),
	    	kvp("rank", 0),
	    	kvp("victories", 0),
	    	kvp("defeats", 0),
	    	kvp("roosters", user_roosters)
	    ));
	}

	bot.on_select_click([=, &bot](const dpp::select_click_t &slc)
		mutable
	{
		dpp::message message = slc.command.get_context_message();
		message.embeds.clear();
		message.components.clear();
		dpp::user slc_author = slc.command.usr;
		string value = slc.values.at(0);

		if(slc.custom_id == "select_edit_rts") {
			selected_rooster = value;
			message.set_content(fmt::format("Selecione abaixo oque você deseja alterar no galo: **{}**", value));
			auto rts_action_menu = dpp::component()
				.set_label("Selecione a ação")
				.set_id("select_rts_action_menu")
				.set_type(dpp::cot_selectmenu)
				.add_select_option(dpp::select_option("Nome", "name", "Edite o novo de um galo"));
			message.add_component(dpp::component().add_component(rts_action_menu));
			bot.message_edit(message);
		}

		if(slc.custom_id == "select_rts_action_menu") {
			message.set_content(fmt::format("Envie abaixo o novo nome do galo: **{}**", selected_rooster));
			bot.message_edit(message);
			new message_collector(&bot, slc_author, "rooster_name", roosters, selected_rooster);
		}

		if(slc.custom_id == "select_remove_rts") {
			message.set_content(fmt::format("**{}** removido com sucesso.", value));
			bot.message_edit(message);
		    user_collection.update_one(
	            make_document(kvp("_id", to_string(slc_author.id))),
	            make_document(kvp("$set", make_document(kvp("roosters", remove_roosters(roosters, value)))))
		    );
		}

		if(slc.custom_id == "select_send_rts") {
			selected_rooster = value;
			message.set_content(fmt::format("Selecione abaixo para quem você deseja enviar o galo: **{}**", value));
			auto rts_action_menu = dpp::component()
				.set_placeholder("Selecione o usuário")
				.set_id("select_send_user_rts")
				.set_type(dpp::cot_user_selectmenu);
			message.add_component(dpp::component().add_component(rts_action_menu));
			bot.message_edit(message);
		}

		if(slc.custom_id == "select_send_user_rts") {
			if(value == to_string(slc_author.id)) {
				message.set_content("Você não pode enviar este galo para si mesmo.");
				bot.message_edit(message);
				return;
			}

			user_collection.update_one(
				make_document(kvp("_id", to_string(slc_author.id))),
				make_document(kvp("$set", make_document(kvp("roosters", remove_roosters(roosters, selected_rooster)))))
			);

			Rooster new_rt;
			for(auto rt : roosters) 
				if(rt.name == selected_rooster) new_rt = rt;

			auto user_to_send = user_collection.find_one(make_document(kvp("_id", value)));
			if(!user_to_send) 
				 user_collection.insert_one(make_document(
					kvp("_id", value),
					kvp("roosters", add_roosters_to_user(3))
				));

			user_collection.update_one(
				make_document(kvp("_id", value)),
				make_document(kvp("$push", make_document(kvp("roosters", rooster_db_struct(new_rt)))))
			);

			message.set_content(fmt::format("**{}** enviado para <@{}> com sucesso.", selected_rooster, value));
			bot.message_edit(message);
		}

		if(slc.custom_id == "select_xbot_rts") {
			for(auto rt : roosters) {
				if(rt.name == value) {
					vector<int> rarity_min_max = get_rarity_in_number(rt.rarity);
					Rooster bot_rt; 
					bot_rt.create(rarity_min_max[0], rarity_min_max[1]);
					in_batle_roosters = {rt, bot_rt};
				}
			}
			turn++;
			dpp::user bot_user = bot.me;
			vector<dpp::user> players = {slc_author, bot_user};
			bot.message_edit(batle_embed(message, players));
		}

		if(slc.custom_id == "select_train_rts") {
			bsoncxx::document::value doc = make_document(
				kvp("roosters", [=](sub_array child) {
					for (auto rt : roosters) {
						if(rt.name == value) {
							rt.atk = rt.atk+randomInt(5, 10);
							rt.def = rt.def+randomInt(5, 10);
							rt.life = rt.life+randomInt(5, 10);
						}
						child.append(rooster_db_struct(rt));
					}
				})
			);

		    message.set_content(fmt::format("**{}** treinado com sucesso", value));
		    bot.message_edit(message);
		    user_collection.update_one(
		    	make_document(kvp("_id", to_string(slc_author.id))),
		    	make_document(kvp("$set", make_document(kvp("roosters", doc.view()["roosters"].get_array().value))))
		    );
		}

		if(slc.custom_id == "select_match_rts") {
			selected_rooster = value;
			message.set_content(fmt::format("Selecione o segundo galo para o match com **{}**", value));
			message.add_component(dpp::component().add_component(generate_rt_menu(roosters, "select_rts_menu_match_2")));
			bot.message_edit(message);
		}

		if(slc.custom_id == "select_skill_rts") {
			dpp::user bot_user = bot.me;
			vector<dpp::user> players = {slc_author, bot_user};
			vector<Rooster> new_rts = batle_action("skill", players, std::stoi(value));
			dpp::message n_msg = batle_embed(batle_message, players);

			for(auto& row : n_msg.components)
				for(auto& x : row.components)
				x.set_disabled(true);

			bot.message_edit(n_msg);
			if(new_rts[1].current_life == 0 || new_rts[1].current_life < 0) {
				batle_ended(batle_message, slc_author, bot, "user");
			} else {
				new_rts = batle_action("", players);
				players = {bot_user, slc_author};

				bot.message_edit(batle_embed(batle_message, players));
				if(new_rts[0].current_life == 0 || new_rts[0].current_life < 0) {
					batle_ended(batle_message, slc_author, bot, "bot");
				}
			}

			bot.message_delete(message.id, message.channel_id);
			return;
		}

		if(slc.custom_id == "select_rts_menu_match_2") {
			int atk; int def; int life;
			bsoncxx::document::value doc = make_document(
				kvp("roosters", [&](sub_array child) {
					for (auto rt : roosters) {
						if(rt.name == selected_rooster) {
							atk = rt.atk;
							def = rt.def;
							life = rt.life;
						}
						if(rt.name == value) {
							rt.name = randomString(12);
							rt.atk = atk+rt.atk; 
							rt.def = rt.def+def; 
							rt.life = rt.life+life;
						}
						child.append(rooster_db_struct(rt));
					}
				})
			);
		    message.set_content("Novo galo criado com sucesso!");
		    bot.message_edit(message);
		    user_collection.update_one(
		    	make_document(kvp("_id", to_string(slc_author.id))),
		    	make_document(kvp("$set", make_document(kvp("roosters", doc.view()["roosters"].get_array().value))))
		    );
		}
		slc.reply(dpp::interaction_response_type::ir_deferred_update_message, dpp::message());
	});

	bot.on_button_click([=, &bot](const dpp::button_click_t &btn)
		mutable
	{
		dpp::message message = btn.command.get_context_message();
		message.embeds.clear(); message.components.clear();

	    int id_divisor = btn.custom_id.find("_");
	    string first_id_arg = btn.custom_id.substr(0, id_divisor);
	    string last_id_arg = btn.custom_id.substr(id_divisor+1);

		auto back_button = dpp::component()
			.set_emoji(u8"⬅")
			.set_type(dpp::cot_button)
			.set_style(dpp::cos_secondary)
			.set_id("rinha_back");
		dpp::user btn_author = btn.command.usr;

		if(btn.custom_id == "rinha_back") 
			bot.message_edit(message.add_embed(mainEmbed(btn_author, bot, roosters)).add_component(mainButtons()));

		if(btn.custom_id == "rinha_galinheiro") {
			auto embed_galinheiro = dpp::embed()
				.set_author(fmt::format("{} • Galinheiro", bot.me.username), "", bot.me.get_avatar_url())
				.set_color(0xed0024)
				.add_field(
					"Galos disponíveis:",
					available_rooters(roosters).append("\n\nPara mais infos sobre um galo, use **/galoinfo**")
				)
				.set_footer(dpp::embed_footer().set_text(btn_author.username).set_icon(btn_author.get_avatar_url()))
				.set_timestamp(time(0));
			message.add_embed(embed_galinheiro);
			message.add_component(dpp::component()
			.add_component(back_button)
			.add_component(generate_button("Editar", dpp::cos_primary, "rinha_edit"))
			.add_component(generate_button("Remover", dpp::cos_primary, "rinha_remove"))
			.add_component(generate_button("Enviar", dpp::cos_primary, "rinha_send")));
			bot.message_edit(message);
		}

		if(btn.custom_id == "rinha_treinamento") {
			auto embed_treinamento = dpp::embed()
				.set_author(fmt::format("{} • Treinamento", bot.me.username), "", bot.me.get_avatar_url())
				.set_color(0xed0024)
				.add_field(
					"Galos disponíveis:",
					available_rooters(roosters)
				)
				.set_footer(dpp::embed_footer().set_text(btn_author.username).set_icon(btn_author.get_avatar_url()))
				.set_timestamp(time(0));
			message.add_embed(embed_treinamento);
			message.add_component(dpp::component()
				.add_component(back_button)
				.add_component(generate_button("Treinar", dpp::cos_primary, "rinha_train"))
				.add_component(generate_button("Mesclar", dpp::cos_primary, "rinha_match")));
			bot.message_edit(message);
		}

		if(btn.custom_id == "rinha_batalha") {
			auto db_historys = db_user->view()["batle_history"].get_array().value;
			auto embed_batalha = dpp::embed()
				.set_author(fmt::format("{} • Batalha", bot.me.username), "", bot.me.get_avatar_url())
				.set_color(0xed0024)
				.add_field(
					"Galos disponíveis:",
					available_rooters(roosters)
				)
				.add_field(
					"Ultímas partidas:",
					batle_history_format(db_historys, 3)
				)
				.set_footer(dpp::embed_footer().set_text(btn_author.username).set_icon(btn_author.get_avatar_url()))
				.set_timestamp(time(0));
			message.add_embed(embed_batalha);
			message.add_component(dpp::component()
				.add_component(back_button)
				.add_component(generate_button("vs bot", dpp::cos_primary, "rinha_xbot"))
				.add_component(generate_button("vs player", dpp::cos_primary, "rinha_xplayer"))
				.add_component(generate_button("Histórico", dpp::cos_primary, "rinha_history")));
			bot.message_edit(message);
		}

		if(btn.custom_id == "rinha_history") {
			auto db_historys = db_user->view()["batle_history"].get_array().value;
			auto embed = dpp::embed()
				.set_author(fmt::format("{} • Histórico", bot.me.username), "", bot.me.get_avatar_url())
				.set_color(0xed0024)
				.add_field(
					"Histórico:",
					batle_history_format(db_historys)
				)
				.set_footer(dpp::embed_footer().set_text(btn_author.username).set_icon(btn_author.get_avatar_url()))
				.set_timestamp(time(0));
			message.add_embed(embed);
			message.add_component(dpp::component().add_component(back_button).set_id("rinha_batalha"));
			bot.message_edit(message);
		}

		if(btn.custom_id == "rinha_edit") {
			btn.reply(dpp::message().add_component(dpp::component().add_component(generate_rt_menu(roosters, "select_edit_rts"))));
			return;
		}
		if(btn.custom_id == "rinha_remove") {
			btn.reply(dpp::message().add_component(dpp::component().add_component(generate_rt_menu(roosters, "select_remove_rts"))));
			return;
		}
		if(btn.custom_id == "rinha_send") {
			btn.reply(dpp::message().add_component(dpp::component().add_component(generate_rt_menu(roosters, "select_send_rts"))));
			return;
		}
		if(btn.custom_id == "rinha_train") {
			btn.reply(dpp::message().add_component(dpp::component().add_component(generate_rt_menu(roosters, "select_train_rts"))));
			return;
		}
		if(btn.custom_id == "rinha_match") {
			btn.reply(dpp::message().add_component(dpp::component().add_component(generate_rt_menu(roosters, "select_match_rts"))));
			return;
		}
		if(btn.custom_id == "rinha_xbot") {
			btn.reply(dpp::message().add_component(dpp::component().add_component(generate_rt_menu(roosters, "select_xbot_rts"))));
			return;
		}

		if(first_id_arg == "xbot") {
			if(last_id_arg == "skill") {
				dpp::message n_msg;
				batle_message = message;
				auto skills_menu = dpp::component()
					.set_placeholder("Selecione uma skill:")
					.set_id("select_skill_rts")
					.set_type(dpp::cot_selectmenu);
				for(auto skill : in_batle_roosters[0].skills) {
					if(skill.limit > 0) {
						skills_menu.add_select_option(dpp::select_option(skill.name, to_string(skill.code), skill.type));
					}
				}
				btn.reply(n_msg.add_component(dpp::component().add_component(skills_menu)));
				return;
			} else if(last_id_arg == "giveup") {
				dpp::user bot_user = bot.me;
				vector<dpp::user> players = {btn_author, bot_user};
				dpp::message n_msg = batle_embed(message, players);
				for(auto& row : n_msg.components)
					for(auto& x : row.components)
					x.set_disabled(true);
				bot.message_edit(n_msg);
				batle_ended(message, btn_author, bot, "giveup");
			} else {
					dpp::user bot_user = bot.me;
					vector<dpp::user> players = {btn_author, bot_user};
					vector<Rooster> new_rts = batle_action(last_id_arg, players);
					dpp::message n_msg = batle_embed(message, players);

					for(auto& row : n_msg.components)
						for(auto& x : row.components)
						x.set_disabled(true);

				    bot.message_edit(n_msg);
				    if(new_rts[1].current_life == 0 || new_rts[1].current_life < 0) {
				    	batle_ended(message, btn_author, bot, "user");
				    } else {
				    	new_rts = batle_action("", players);
					    players = {bot_user, btn_author};

					    bot.message_edit(batle_embed(message, players));
					    if(new_rts[0].current_life == 0 || new_rts[0].current_life < 0) {
					    	batle_ended(message, btn_author, bot, "bot");
					    }
				    }
				}
		}
		btn.reply(dpp::interaction_response_type::ir_deferred_update_message, dpp::message());
	});

	dpp::embed m_embed = mainEmbed(author, bot, roosters);
	dpp::component m_buttons = mainButtons();
	dpp::message msg;
	msg.add_embed(m_embed);
	msg.add_component(m_buttons);
	event.reply(msg);
}
