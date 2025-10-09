#include "iostream"
#include "commands/stats.hpp"

void StatsCommand::Execute() {
    std::cout << m_author.username << "\n";
}