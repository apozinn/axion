#pragma once
#include "command.hpp"

class InventoryCommand : public Command {
public:
    InventoryCommand(const dpp::slashcommand_t& event)
        : Command(event) {}

    void Execute() override;
};