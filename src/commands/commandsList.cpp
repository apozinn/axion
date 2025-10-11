#include "commands/commandsList.hpp"
#include "i18nManager/i18nManager.hpp"

namespace CommandsList
{
    std::vector<CommandStruct> SlashCommandsList{
        // Utility & Core
        {"help", "Displays detailed command information."},
        {"history", "Shows a log of your recent actions and battles."},

        // Profile & Roster
        {"coop", "Displays your full collection of roosters (your roster/party)."},
        {"stats", "Shows the detailed stats of your currently selected rooster."},
        {"select", "Sets a specific rooster from your collection as the active fighter."},

        // Progression & Acquisition
        {"hunt", "Attempts to find and catch a new wild rooster."},
        {"train", "Spends resources to increase the stats and XP of the active rooster."},
        {"moves", "Shows the list of available skills/abilities for your active rooster."},

        // Economy & Action
        {"fight", "Initiates a battle with another player (PvP) or a random encounter."},
        {"heal", "Restores your rooster's health using items or a cooldown."},
        {"shop", "Opens the market to buy and sell items."},
        {"inventory", "Displays all items and resources currently owned by the user."},

        // Championship
        {"league", "Initiates a progressive battle against strong AI opponents (PvE)."},
        {"champ", "Manages the server's competitive PvP tournament (subcommands: open, join)."}
    };
}