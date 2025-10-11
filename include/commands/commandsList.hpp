#pragma once

#include <vector>
#include <string>

namespace CommandsList
{
    struct CommandStruct
    {
        std::string name;
        std::string description; 
    };

    extern std::vector<CommandStruct> SlashCommandsList;
}