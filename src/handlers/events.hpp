#pragma once

#include <iostream>
#include <dpp/dpp.h>
#include <string>
#include "spdlog/spdlog.h"

using namespace std;
using namespace dpp;

#include "../events/ready.hpp"
#include "../events/interactionCreate.hpp"
#include "../events/messageCreate.hpp"

inline void LoadEvents(cluster& bot) {
	ready(bot);
	interactionCreate(bot);
	messageCreate(bot);
	return;
}