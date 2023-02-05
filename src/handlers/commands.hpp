#pragma once

#include <iostream>
#include <string>
#include <dpp/dpp.h>

using namespace std;

std::vector<std::vector<std::string>> commands {
	{ "rinha", "Batalhas incríveis com galos!" },
	{ "galoinfo", "Vejas as informações de um galo" },
	{ "rank", "Veja sua colocação entre todos os treinadores" },
	{ "shop", "Compre e venda itens relacionados as galos" }
};

void LoadCommands(dpp::cluster& bot) {
	std::vector<dpp::slashcommand> slashCmds;
	for (auto& n_cmd : commands) {
		dpp::slashcommand cmd;
		cmd.set_name(n_cmd[0])
			.set_description(n_cmd[1])
			.set_application_id(bot.me.id);
		slashCmds.push_back(cmd);
	}
	bot.global_bulk_command_create(slashCmds);
}