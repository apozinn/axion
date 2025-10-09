#pragma once
#include "commands/command.hpp"

class HelpCommand : public Command {
public:
    HelpCommand(const dpp::slashcommand_t& event)
        : Command(event) {}

    void Execute() override;
};