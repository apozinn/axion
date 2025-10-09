#include "iostream"
#include "commands/skills.hpp"

void SkillsCommand::Execute() {
    std::cout << m_author.username << "\n";
}