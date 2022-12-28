#pragma once

#include <iostream>
#include <dpp/dpp.h>
#include <string>
#include "spdlog/spdlog.h"

using namespace std;
using namespace dpp;

void messageCreate(cluster& bot) {
	bot.on_message_create([&bot](const message_create_t& event) {
		const auto message = event.msg;
	});
	return;
}