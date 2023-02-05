#pragma once

#include <iostream>
#include <string>
#include <dpp/dpp.h>
#include "../events/interactionCreate.hpp"

using namespace std;

void LoadEvents(dpp::cluster& bot) {
	interactionCreate(bot);
}