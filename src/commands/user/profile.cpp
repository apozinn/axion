#include <iostream>
#include <dpp/dpp.h>
#include "spdlog/spdlog.h"
#include <string>

using namespace std;
using namespace dpp;

void Profile(dpp::cluster& bot, const dpp::slashcommand_t& event, dpp::guild& guild, user& author) {
	event.reply("Profile");
	return;
}