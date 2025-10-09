#pragma once
#include "vector"
#include "command.hpp"
#include "rooster/rooster.hpp"
#include "dpp/dpp.h"

class FightCommand : public Command {
public:
    FightCommand(const dpp::slashcommand_t& event)
        : Command(event) {}

    void Execute() override;

    private:
    dpp::embed MainEmbed();
    dpp::component MainButtons();
    std::vector<Rooster> m_userRoosters;
};