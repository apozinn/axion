#pragma once

#include <vector>
#include "fmt/core.h"
#include "rooster/rooster.hpp"

/**
 * @namespace RoosterUtils
 * @brief Provides utility functions for creating and managing Rooster objects.
 */
namespace RoosterUtils
{

    /**
     * @brief Creates a vector of user-owned roosters.
     * @param length The number of roosters to create.
     * @return A vector containing the created Rooster objects.
     *
     * Each rooster in the vector is initialized with default or randomized properties.
     */
    std::vector<Rooster> CreateUserRooster(int length);

    std::string AvailableRooters(std::vector<Rooster> &roosters);
}
