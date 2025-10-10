#pragma once

#include <dpp/cluster.h>
#include "configManager/configManager.hpp"
#include "i18nManager/i18nManager.hpp"

/**
 * @class Client
 * @brief Represents the Discord bot client, handling connection, events, and slash commands.
 *
 * The Client class encapsulates the setup and interaction with the DPP Discord library,
 * managing bot connection, event binding, and command handling. It also provides access
 * to configuration and internationalization managers.
 */
class Client
{
public:
    /**
     * @brief Connects the bot to Discord.
     *
     * Initializes the DPP cluster with the configured token, binds event handlers,
     * registers slash commands, and starts the bot.
     */
    void Connect();

private:
    /**
     * @brief Binds event handlers for the bot.
     *
     * Registers callbacks for Discord events such as slash commands and message creation.
     */
    void BindEvents();

    /**
     * @brief Registers all bot slash commands.
     *
     * Iterates through the list of defined slash commands and registers them with Discord.
     */
    void RegisterSlashCommands();

    void OnReady(const dpp::ready_t &event);

    /**
     * @brief Handles a slash command interaction.
     * @param event The slash command event received from Discord.
     *
     * This function is called when a user invokes a slash command. It dispatches
     * the command to the appropriate command handler.
     */
    void OnInteractionCreate(const dpp::slashcommand_t &event);

    /**
     * @brief Handles message creation events.
     * @param event The message creation event received from Discord.
     *
     * Processes messages sent in channels the bot has access to, for example
     * detecting mentions or responding to messages.
     */
    void OnMessageCreate(const dpp::message_create_t &event);

    /** 
     * @brief Pointer to the configuration manager singleton.
     * Provides access to application configuration values.
     */
    ConfigManager* m_config = &ConfigManager::Get();

    /** 
     * @brief Pointer to the internationalization manager singleton.
     * Provides localized strings and translations.
     */
    i18nManager* m_i18n = &i18nManager::Get();

    /**
     * @brief Pointer to the DPP cluster instance.
     *
     * Manages the connection to Discord and handles event dispatch.
     */
    dpp::cluster *m_cluster;
};
