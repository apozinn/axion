#pragma once
#include "command.hpp"

class SkillsCommand : public Command {
public:
    SkillsCommand(const dpp::slashcommand_t& event)
        : Command(event) {}

    void Execute() override;
};