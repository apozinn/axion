#pragma once
#include "command.hpp"

class StatsCommand : public Command {
public:
    StatsCommand(const dpp::slashcommand_t& event)
        : Command(event) {}

    void Execute() override;
};