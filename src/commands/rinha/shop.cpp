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

void Shop(
	dpp::cluster &bot, 
	const dpp::slashcommand_t &event, 
	dpp::guild &guild, 
	dpp::user &author
) {
	auto client = mongo_access::instance().get_connection();
    auto user_collection = (*client)["axion"]["users"];
    auto db_user = user_collection.find_one(make_document(kvp("_id", to_string(author.id))));

    auto embed = dpp::embed()
    	.set_author(fmt::format("{} • Shop", bot.me.username), "", bot.me.get_avatar_url())
    	.set_color(0xed0024)
    	.set_description(fmt::format("Shop"))
    	.set_timestamp(time(0));
    event.reply(dpp::message().add_embed(embed));
}