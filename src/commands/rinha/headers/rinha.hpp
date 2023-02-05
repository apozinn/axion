#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <dpp/dpp.h>
#include <cctype>
#include <random>
#include <algorithm>
#include "spdlog/spdlog.h"

#include "../../../database/db.hpp"
#include "../../../utils/randoms.hpp"
#include "./skills.hpp"
#include "./rooster.hpp"
#include "./rewards.hpp"

using namespace std;

vector<Rooster> in_batle_roosters;
vector<string> batle_history;
dpp::message batle_message;
int turn = 0;

auto use_skill(vector<Rooster>& rts, Skill& skill) {
    Rooster rt_requester = rts[0];
    Rooster rt_opponent = rts[1];

    switch(skill.code) {
        case 1: {
            rt_opponent.current_life = rt_opponent.current_life-40;
        } break;
    }

    vector<Rooster> returned_rts = {rt_requester, rt_opponent};
    return returned_rts;
}

auto get_rarity_in_number = [](string rarity) {
	vector<int> rarity_min_max;
	if(rarity == "Comum") rarity_min_max = {0, 40};
	if(rarity == "Raro") rarity_min_max = {41, 65};
	if(rarity == "Epíco") rarity_min_max = {66, 80};
	if(rarity == "Lendário") rarity_min_max = {81, 90};
	if(rarity == "Mitíco") rarity_min_max = {91, 98};
	if(rarity == "Celestial") rarity_min_max = {99, 100};
	return rarity_min_max;
};

auto skill_struct_db(Skill& skill) {
	bsoncxx::document::value doc = make_document(
		kvp("name", skill.name),
		kvp("type", skill.type),
		kvp("code", skill.code),
		kvp("limit", skill.limit),
		kvp("description", skill.description)
	);
	return doc.view();
}

auto rooster_db_struct(Rooster& rt) 
{
	bsoncxx::document::value doc = make_document(
		kvp("name", rt.name),
		kvp("classe", rt.classe),
		kvp("rarity", rt.rarity),
		kvp("weakness", rt.weakness),
		kvp("life", rt.life),
		kvp("atk", rt.atk),
		kvp("def", rt.def),
		kvp("stamina", rt.stamina),
		kvp("velocity", rt.velocity),
		kvp("level", rt.level),
		kvp("moral", rt.moral),
		kvp("skills", [=](sub_array child) {
    		for (auto skl : rt.skills) {
        		child.append(skill_struct_db(skl));
    		}
		})
	);
	return doc.view();
}

void create_batle_history(
	vector<Rooster>& batle_rts, 
	dpp::user& user_one, 
	dpp::user& user_two,
	Rewards& rewards,
	string winner
) {
	auto client = mongo_access::instance().get_connection();
	auto user_collection = (*client)["axion"]["users"];

	user_collection.update_one(
		make_document(kvp("_id", to_string(user_one.id))),
		make_document(kvp("$push", make_document(kvp("batle_history", make_document(
			kvp("winner", winner),
			kvp("date", time(0)),
			kvp("turns", turn),
			kvp("rewards", make_document(
				kvp("corns", rewards.corn),
				kvp("user_xp", rewards.user_xp),
				kvp("uprank", rewards.uprank),
				kvp("golden_spike", rewards.golden_spike),
				kvp("box", rewards.box),
				kvp("rooster_xp", rewards.rooster_xp)
			)),
			kvp("roosters", [&](sub_array child) {
				for(auto rt : batle_rts) {
					child.append(rt.name);
				}
			})
		)))
	)));
}

string available_rooters(vector<Rooster> &roosters) {
	string rts;
	for(auto rt: roosters) {
		rts.append(
			fmt::format("{}**{}** ({} - {})\n[ life: {} | atk: {} | def: {} | level: {} ]", 
			!rts.size() ? "" : "\n",
			rt.name,
			rt.classe,
			rt.rarity,
			to_string(rt.life),
			to_string(rt.atk), 
			to_string(rt.def),
			to_string(rt.level)
		));
	}
	return rts;
}

dpp::component generate_button(string label, const dpp::component_style& style, string _id)
{
	auto button = dpp::component()
		.set_label(label)
		.set_type(dpp::cot_button)
		.set_style(style)
		.set_id(_id);
	return button;
}

dpp::component generate_rt_menu(vector<Rooster>& roosters, string id) {
	auto menu = dpp::component()
		.set_placeholder("Selecione o galo desejado:")
		.set_id(id)
		.set_type(dpp::cot_selectmenu);
	for(auto rt : roosters) {
		menu.add_select_option(dpp::select_option(rt.name, rt.name, rt.rarity));
	}
	return menu;
}

dpp::embed mainEmbed(
	dpp::user &author, 
	dpp::cluster& bot, 
	vector<Rooster>& roosters
) {
	dpp::embed embed = dpp::embed()
		.set_author(bot.me.username, "", bot.me.get_avatar_url())
		.set_thumbnail("https://imgur.com/RlMdIan.png")
		.set_color(0xed0024)
		.set_description(fmt::format("Treinador: {}(**{}**)\nRank: **{}^**/{}(**{}#**)\nPartidas: {}({}w / {}l)\nTítulo: {}",
			author.username,
			author.id,
			to_string(0),
			"Galo júnior",
			to_string(100),
			to_string(10),
			to_string(7),
			to_string(3),
			"O corajoso"
		))
		.add_field(
			"Galos disponíveis",
			available_rooters(roosters).append("\n\nCom duvídas ou problemas? use **/sobre**")
		)
		.set_footer(dpp::embed_footer().set_text(author.username).set_icon(author.get_avatar_url()))
		.set_timestamp(time(0));
	return embed;
}

dpp::component mainButtons()
{
	dpp::component cmpt = dpp::component();
	cmpt.add_component(
		generate_button("Galinheiro", dpp::cos_success, "rinha_galinheiro")
	).add_component(
	generate_button("Treinamento", dpp::cos_success, "rinha_treinamento")
	).add_component(
	generate_button("Batalha", dpp::cos_success, "rinha_batalha"));
	return cmpt;
}

class message_collector : public dpp::message_collector {
	public:
		dpp::user user_filter;
		string action_type;
		string selected_rooster;
		vector<Rooster> roosters;
	message_collector(
		dpp::cluster *cl, 
		dpp::user &user, 
		string action, 
		vector<Rooster>& rts,
		string slt_rt
	) : dpp::message_collector(cl, 30)
	{
		user_filter = user;
		action_type = action;
		roosters = rts;
		selected_rooster = slt_rt;
	}

	virtual void completed(const std::vector<dpp::message> &list)
	{
		auto client = mongo_access::instance().get_connection();
		auto user_collection = (*client)["axion"]["users"];
		
		if (list.size()) {
			if(action_type == "rooster_name") {
				bsoncxx::document::value doc = make_document(
					kvp("roosters", [=](sub_array child) {
						for (auto rt : roosters) {
							if(rt.name == selected_rooster) rt.name = list[0].content;
							child.append(rooster_db_struct(rt));
						}
					})
				);

				owner->message_create(dpp::message(list[0].channel_id, fmt::format("Nome do galo alterado para: **{}**", list[0].content)));
			    user_collection.update_one(
			    	make_document(kvp("_id", to_string(user_filter.id))),
			    	make_document(kvp("$set", make_document(kvp("roosters", doc.view()["roosters"].get_array().value))))
			    );
			}
		} else owner->message_create(dpp::message("Nenhuma mensagem recebida, cancelando operação."));
	}

	virtual const dpp::message *filter(const dpp::message_create_t &m)
	{
		if (m.msg.author.id == user_filter.id)
			return &m.msg;
		else return nullptr;
	}
};

vector<Rooster> batle_action(
	string action, 
	vector<dpp::user>& players,
	int selected_skill_code=0
) {
	Rooster rt_requester;
	Rooster rt_opponent;
	dpp::user user_requester;
	turn++;

	auto rt_recollation = [&](Rooster& rt) {
		vector<int> min_max = {10, 30};
		if(rt.classe == "Raro") min_max = {20, 40};
		if(rt.classe == "Epíco") min_max = {30, 50};
		if(rt.classe == "Lendário") min_max = {40, 60};
		if(rt.classe == "Mitíco") min_max = {50, 70};
		if(rt.classe == "Celestial") min_max = {60, 80};
		return randomInt(min_max[0], min_max[1]);
	};

	if(action.size()) {
		rt_requester = in_batle_roosters[0];
		rt_opponent = in_batle_roosters[1];
		user_requester = players[0];
		if(randomInt(1, 100) <= 10) {
			int remove_moral = randomInt(1, 10);
			rt_requester.moral-remove_moral;
			batle_history.insert(batle_history.begin(), fmt::format("**{}** falhou e perdeu `{}` pontos de moral ☠️", user_requester.username, to_string(remove_moral)));
			in_batle_roosters = {rt_requester, rt_opponent};
			return in_batle_roosters;
		}
	} else {
		int random_action = randomInt(0, 3);
		rt_requester = in_batle_roosters[1];
		rt_opponent = in_batle_roosters[0];
		user_requester = players[1];
		if(random_action == 0) action = "attack";
		if(random_action == 1) action = "defend";
		if(random_action == 2) action = "heal";
		if(random_action == 3) {
			Skill bot_skill = rt_requester.skills.at(randomInt(0, rt_requester.skills.size()-1));
			batle_history.insert(batle_history.begin(), fmt::format("**{}** usou a skill `{}` 🧬", user_requester.username, bot_skill.name));
		}
	}

	if(action == "attack") {
		int damage;
		if(rt_opponent.def > rt_requester.atk) {
			damage = rt_requester.atk/2;
		} else {
			damage = rt_requester.atk-rt_opponent.def;
		}
		rt_opponent.def = rt_opponent.def-rt_requester.atk;
		if(rt_opponent.def < 0) rt_opponent.def = 0;
		rt_opponent.current_life = rt_opponent.current_life-damage;
		if(rt_opponent.current_life < 0) rt_opponent.current_life = 0;
		batle_history.insert(batle_history.begin(), fmt::format("**{}** usou um ataque de `{}` de dano 🗡️", user_requester.username, to_string(damage)));
	}
	if(action == "defend") {
		int defense = rt_recollation(rt_requester);
		rt_requester.def = rt_requester.def+defense;
		batle_history.insert(batle_history.begin(), fmt::format("**{}** aumentou sua defesa em `{}` 🛡️", user_requester.username, to_string(defense)));
	}
	if(action == "heal") {
		int heal = rt_recollation(rt_requester);
		rt_requester.current_life = rt_requester.current_life+heal;
		if(rt_requester.current_life > rt_requester.life) rt_requester.current_life = rt_requester.life;
		batle_history.insert(batle_history.begin(), fmt::format("**{}** curou `{}` pontos de vida 🧪", user_requester.username, to_string(heal)));
	}
	if(action == "skip") {
		batle_history.insert(batle_history.begin(), fmt::format("**{}** pulou a rodada ➡", user_requester.username));
	}
	if(action == "skill") {
		Skill skill;
		vector<Skill> new_skills;
		for(auto skl : rt_requester.skills) {
			if(selected_skill_code == skl.code) {
				skl.limit = skl.limit-1;
				skill = skl;
				new_skills.push_back(skl);
			} else new_skills.push_back(skl);
		}

		rt_requester.skills = new_skills;
		vector<Rooster> in_skills_rts = {rt_requester, rt_opponent};
		vector<Rooster> returned_rts = use_skill(in_skills_rts, skill);
		rt_requester = returned_rts[0];
		rt_opponent = returned_rts[1];
		batle_history.insert(batle_history.begin(), fmt::format("**{}** usou a skill `{}` 🧬", user_requester.username, skill.name));
	}

	if(user_requester.is_bot()) {
		in_batle_roosters = {rt_opponent, rt_requester};
	} else {
		in_batle_roosters = {rt_requester, rt_opponent};
	}
	return in_batle_roosters;
}

int firstDigit(int n) {
	while (n >= 10) n /= 10;
	return n;
}

string life_block(float total_life, float current_life) {
	string life_block_string = "[";
	int per_y;

	if(total_life == current_life) {
		per_y = 30;
	} else {
		float per_x = (current_life/total_life)*100;
		if(per_x >= 95) {
			per_y = 29;
		} else {
			per_y = firstDigit(static_cast<int>(per_x))*3;
			if(per_y < 3) per_y = 1;
		}
	}

	for (int i = 0; i < per_y; ++i)
	{
		life_block_string.append("=");
	}

	for (int i = 0; i < (30-per_y); ++i)
	{
		life_block_string.append("-");
	}

	life_block_string.append(fmt::format("] {}/{}", current_life, total_life));
	return life_block_string;
}

string history_formatter() {
	string hts;
	int l = 0;
	for(auto ht : batle_history) {
		if(l < 4) {
			hts.append(fmt::format("{}{}", !hts.size() ? "" : "\n", ht));
		}
		l++;
	}
	return hts;
}

dpp::message batle_embed(
	dpp::message& message,
	vector<dpp::user>& players
) {
	string description;
	message.embeds.clear(); message.components.clear();
	for(auto rt : in_batle_roosters) {
		description.append(fmt::format("{}\n\n**{}**\n{}\nATK: {} DEF: {} CLASS: {} RARITY: {}", 
			!description.size() ? fmt::format("Jogador da rodada: <@{}>", players[1].id) : "",
			rt.name,
			life_block(rt.life, rt.current_life),
			to_string(rt.atk),
			to_string(rt.def),
			rt.classe,
			rt.rarity
		));
	}

	dpp::embed embed = dpp::embed()
		.set_title(fmt::format("{} vs {}", players[0].username, players[1].username))
		.set_color(0xed0024)
		.set_description(description)
		.set_footer(dpp::embed_footer().set_text(fmt::format("rodada {} | partida contra bot", turn)));
	if(batle_history.size()) {
		embed.add_field(
			"Ultímas jogadas",
			history_formatter()
		);
	}
	message.add_embed(embed);
	message.add_component(dpp::component()
		.add_component(generate_button("Atacar", dpp::cos_secondary, "xbot_attack").set_emoji(u8"🗡️"))
		.add_component(generate_button("Defesa", dpp::cos_secondary, "xbot_defend").set_emoji(u8"🛡️"))
		.add_component(generate_button("Cura", dpp::cos_secondary, "xbot_heal").set_emoji(u8"🧪")));
	message.add_component(dpp::component()
		.add_component(generate_button("Pular", dpp::cos_secondary, "xbot_skip").set_emoji(u8"➡"))
		.add_component(generate_button("Skill", dpp::cos_secondary, "xbot_skill").set_emoji(u8"🧬"))
		.add_component(generate_button("Desistir", dpp::cos_danger, "xbot_giveup").set_emoji(u8"🏳️")));
	return message;
}

void batle_ended(
	dpp::message& message,
	dpp::user& author,
	dpp::cluster& bot,
	string winner
) {
	auto client = mongo_access::instance().get_connection();
	auto user_collection = (*client)["axion"]["users"];
	auto author_db = user_collection.find_one(make_document(kvp("_id", to_string(author.id))));
	bsoncxx::document::view db_user = author_db->view();
	auto embed_ended = dpp::embed();

	for(auto& row : message.components)
		for(auto& x : row.components)
		x.set_disabled(true);
	bot.message_edit(message);

	auto back_button = dpp::component()
		.set_emoji(u8"⬅")
		.set_type(dpp::cot_button)
		.set_style(dpp::cos_secondary)
		.set_id("rinha_batalha");
	Rewards reward;
	reward.create(in_batle_roosters[1].rarity);

	dpp::user bot_user = bot.me;
	create_batle_history(in_batle_roosters, author, bot_user, reward, winner);

	auto db_corns = db_user["corns"].get_int32().value;
	auto db_user_xp = db_user["xp"].get_int32().value;
	auto db_rank = db_user["rank"].get_int32().value;
	auto db_golden_spike = db_user["golden_spike"].get_int32().value;
	auto db_victories = db_user["victories"].get_int32().value;
	auto db_defeats = db_user["defeats"].get_int32().value;

	if(winner == "user") {
		embed_ended
			.set_author("Vitória!", "", author.get_avatar_url())
			.set_color(dpp::colors::green)
			.set_description(fmt::format("Parabéns por sua gloriosa vitória, aquí estão suas recompensas:\nMilhos: **{}**\nXp: **^{}**\nXp(galo): **^{}**\nRank: **+{}**\nEspígas: **{}**\nBaús: {}",
				to_string(reward.corn),
				to_string(reward.user_xp),
				to_string(reward.rooster_xp),
				to_string(reward.uprank),
				to_string(reward.golden_spike),
				reward.box.size() ? fmt::format("1x **{}**", reward.box) : "**Nehuma**"
			))
			.set_footer(dpp::embed_footer().set_text(fmt::format("Vitórias: {}", to_string(db_victories+1))));
	} 
	if(winner == "bot") {
		embed_ended
			.set_author("Derrota...", "", bot.me.get_avatar_url())
			.set_color(dpp::colors::red)
			.set_description(fmt::format("Força guerreiro, mais sorte na proxíma. Aquí estão suas recompensas:\nMilhos: **{}**\nXp: **{}**\nXp(galo): **{}**",
				to_string(reward.corn/2),
				to_string(reward.user_xp/2),
				to_string(reward.rooster_xp/2)
			))
			.set_footer(dpp::embed_footer().set_text(fmt::format("Derrotas: {}", to_string(db_defeats+1))));
	}
	if(winner == "giveup") {
		embed_ended
			.set_author("Partida abandonada.", "", author.get_avatar_url())
			.set_color(dpp::colors::red)
			.set_description("Partida encerada por desistência, nenhuma recompensa será dada.")
			.set_footer(dpp::embed_footer().set_text(fmt::format("Derrotas: {}", to_string(db_defeats+1))));
	}

	in_batle_roosters.clear();
	batle_history.clear();
	turn = 0;
	bot.message_create_sync(
		dpp::message(message.channel_id, embed_ended).add_component(dpp::component().add_component(back_button))
	);

	if(winner == "giveup") {
		user_collection.update_one(
			make_document(kvp("_id", to_string(author.id))),
			make_document(kvp("$set", make_document(kvp("defeats", db_defeats+1))))
		);
	} else {
		user_collection.update_one(
			make_document(kvp("_id", to_string(author.id))),
			make_document(kvp("$set", make_document(
				kvp("corns", db_corns+reward.corn),
				kvp("xp", db_user_xp+reward.user_xp),
				kvp("victories", db_victories+1),
				kvp("rank", db_rank+reward.uprank),
				kvp("golden_spike", reward.golden_spike ? db_golden_spike+1 : db_golden_spike)
			)))
		);
	}
}

auto batle_history_format = [](auto& element, int limit=0) {
	string string_formated;
	auto add_to_string = [&](auto& ele) {
		string_formated.append(fmt::format("\n**{}** vs **{}**\nVencedor: **{}** | Turnos: **{}** | Data: **{}**",
			ele["roosters"].get_array().value[0].get_string().value.to_string(),
			ele["roosters"].get_array().value[1].get_string().value.to_string(),
			ele["winner"].get_string().value.to_string(),
			to_string(ele["turns"].get_int32().value),
			"Hoje"
		));
	};

	if(limit) {
		for (int i = 0; i < limit; ++i) {
			auto ele = element[i];
			if(ele) add_to_string(ele);
		}
	} else {
		for(auto ele : element) add_to_string(ele);
	}
	return string_formated;
};