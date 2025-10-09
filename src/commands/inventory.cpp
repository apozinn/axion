#include "iostream"
#include "commands/inventory.hpp"

void InventoryCommand::Execute() {
    std::cout << m_author.username << "\n";
}