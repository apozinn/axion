#include "iostream"
#include "commands/help.hpp"

void HelpCommand::Execute() {
    std::cout << m_author.username << "\n";
}