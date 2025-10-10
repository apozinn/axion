#pragma once

#include <string>
#include <vector>
#include <bsoncxx/types.hpp>
#include "skills/skills.hpp"
#include <dpp/dpp.h>

/**
 * @class Rooster
 * @brief Represents a rooster character in the game.
 *
 * The Rooster class stores all relevant attributes for a rooster, 
 * including stats, skills, and properties. It provides functions 
 * to initialize a rooster from BSON array elements.
 */
class Rooster
{
public:
    std::string name;
    std::string classe;
    std::string rarity;
    std::string weakness;
    std::vector<Skill> skills;
    std::vector<int> states;
    int life;
    int current_life;
    int atk;
    int def;
    int stamina;
    int velocity;
    int level;
    int moral;

    /**
     * @brief Initializes the rooster's properties from a BSON element.
     * @param ele The BSON array element containing the rooster data.
     */
    void CreateWithProps(bsoncxx::array::element& ele);
};
