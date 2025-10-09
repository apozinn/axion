#pragma once
#include <string>
#include <cstdint>

/**
 * @file random.hpp
 * @brief Provides utility functions for generating random integers and strings.
 */

using u32 = uint_least32_t;

/**
 * @brief Generates a random integer within a specified range.
 * @param min The minimum value of the range (inclusive).
 * @param max The maximum value of the range (inclusive).
 * @return A random integer between min and max.
 */
u32 randomInt(int min, int max);

/**
 * @brief Generates a random alphanumeric string.
 * @param length The length of the string to generate (default is 32 if 0).
 * @return A randomly generated string containing only allowed characters.
 *
 * The allowed characters exclude visually confusing ones (like 'O', 'I', '0').
 */
std::string randomString(size_t length = 0);
