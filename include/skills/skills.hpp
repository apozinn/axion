#pragma once

#include <string>
#include <vector>
#include <dpp/dpp.h>
#include <random>
#include <algorithm>

/**
 * @class Skill
 * @brief Represents a skill or ability that a rooster can have.
 *
 * Stores basic information about a skill including name, type, code, 
 * usage limit, and description. Provides a utility function to generate
 * skills for a rooster based on class and luck.
 */
class Skill {
public:
    std::string name;
    std::string type;
    int code;
    int limit;
    std::string description;

    /**
     * @brief Creates a vector of skills for a rooster.
     * @param luck A value representing the luck factor for skill generation.
     * @param classe The class/type of the rooster which may affect skill selection.
     * @return A vector of Skill objects generated for the rooster.
     */
    std::vector<Skill> CreateSkills(int luck, const std::string& classe);
};
