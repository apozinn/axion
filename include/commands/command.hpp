#pragma once
#include <dpp/dpp.h>

/**
 * @class Command
 * @brief Abstract base class for all bot commands.
 *
 * The Command class provides a unified interface for handling slash commands.
 * Each derived command must implement the Execute() method. The class stores
 * references to the slash command event, the author, the guild, and the client
 * (cluster) that received the event.
 */
class Command
{
public:
    /**
     * @brief Constructs a Command instance.
     * @param event The Discord slash command event associated with this command.
     *
     * Initializes references to the event, author, guild, and client (cluster)
     * for use in derived classes.
     */
    Command(const dpp::slashcommand_t &event)
        : m_event(event),
          m_client(event.owner),
          m_author(event.command.get_issuing_user()),
          m_guild(event.command.get_guild())
    {}

    /**
     * @brief Virtual destructor.
     *
     * Ensures proper cleanup of derived command classes.
     */
    virtual ~Command() = default;

    /**
     * @brief Executes the command logic.
     *
     * Must be implemented by derived classes to define specific command behavior.
     */
    virtual void Execute() = 0;

protected:
    /**
     * @brief Reference to the original slash command event.
     */
    const dpp::slashcommand_t &m_event;

    /**
     * @brief Pointer to the cluster (bot client) that received the event.
     *
     * This allows derived commands to interact with Discord via the cluster,
     * for example sending messages or modifying guild state.
     */
    dpp::cluster* m_client;

    /**
     * @brief Reference to the user who issued the command.
     */
    const dpp::user &m_author;

    /**
     * @brief Reference to the guild where the command was invoked.
     */
    const dpp::guild &m_guild;
};
