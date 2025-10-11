#include "iostream"
#include <string>

#include "dpp/dpp.h"
#include "fmt/core.h"

#include "commands/help.hpp"
#include "commands/commandsList.hpp"
#include "i18nManager/i18nManager.hpp"

void HelpCommand::Execute()
{
    dpp::embed Embed;
    dpp::message callbackMessage;
    std::string commandsInString;

    for (auto &&command : CommandsList::SlashCommandsList)
    {
        commandsInString.append(
            std::string(
                commandsInString.length() == 0 ? "" : "\n") +
            fmt::format("**/{}** - {}", command.name, i18nManager::Get().GetString(fmt::format("commands.{}.metadata.description", command.name))));
    }

    Embed.add_field(i18nManager::Get().GetString("commands.help.embed_data.title"), commandsInString);
    callbackMessage.add_embed(Embed);
    m_event.reply(callbackMessage);
}