#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <unordered_map>
#include <memory>

#include <dpp/nlohmann/json.hpp>
#include "client/client.hpp"

#include "commands/command.hpp"
#include "commands/fight.hpp"
#include "commands/help.hpp"
#include "commands/inventory.hpp"
#include "commands/skills.hpp"
#include "commands/stats.hpp"

using json = nlohmann::json;

std::vector<std::vector<std::string>> SlashCommandsList{
    {"fight", "Starts a rooster battle by challenging another player."},
    {"inventory", "Views your current collection of roosters and items."},
    {"stats", "Shows your personal statistics and the global leaderboard."},
    {"skills", "Displays all available skills in the game."},
    {"help", "Displays detailed command information."}};

std::unordered_map<std::string, std::function<std::unique_ptr<Command>(const dpp::slashcommand_t &)>> CommandRegistry{
    {"fight", [](const dpp::slashcommand_t &event)
     { return std::make_unique<FightCommand>(event); }},
    {"help", [](const dpp::slashcommand_t &event)
     { return std::make_unique<HelpCommand>(event); }},
    {"inventory", [](const dpp::slashcommand_t &event)
     { return std::make_unique<InventoryCommand>(event); }},
    {"skills", [](const dpp::slashcommand_t &event)
     { return std::make_unique<SkillsCommand>(event); }},
    {"stats", [](const dpp::slashcommand_t &event)
     { return std::make_unique<StatsCommand>(event); }}};

void Client::Connect()
{
    std::string token = m_config->getValue<std::string>("token");
    if (!token.length())
    {
        std::cout << i18nManager::Get().GetString("errors.tokenNotFound") << "\n";
        return;
    }

    m_cluster = new dpp::cluster(token, dpp::i_default_intents | dpp::i_message_content);
    m_cluster->on_log(dpp::utility::cout_logger());

    BindEvents();

    m_cluster->start(dpp::st_wait);
}

void Client::BindEvents()
{
    m_cluster->on_ready([this](const dpp::ready_t &event)
                        { OnReady(event); });
    m_cluster->on_slashcommand([this](const dpp::slashcommand_t &event)
                               { OnInteractionCreate(event); });
    m_cluster->on_message_create([this](const dpp::message_create_t &event)
                                 { OnMessageCreate(event); });
}

void Client::RegisterSlashCommands()
{
    std::vector<dpp::slashcommand> Commands;

    for (auto &n_cmd : SlashCommandsList)
    {
        dpp::slashcommand cmd;
        cmd.set_name(n_cmd[0])
            .set_description(n_cmd[1])
            .set_application_id(m_cluster->me.id);
        Commands.push_back(cmd);
    }

    m_cluster->global_commands_get([this, Commands](const dpp::confirmation_callback_t &callback)
                                   {
    if (callback.is_error()) {
        m_cluster->log(dpp::ll_error, "Failed to fetch global commands: " + callback.http_info.body);
        return;
    }

    auto existing = std::get<dpp::slashcommand_map>(callback.value);
    bool needsUpdate = false;

    if (existing.size() != Commands.size()) {
        needsUpdate = true;
    } else {
        for (const auto& newCmd : Commands) {
            auto it = existing.find(newCmd.name);
            if (it == existing.end() || it->second.description != newCmd.description) {
                needsUpdate = true;
                break;
            }
        }
    }

    if (needsUpdate) {
        m_cluster->log(dpp::ll_info, "Updating global slash commands...");
        m_cluster->global_bulk_command_create(Commands, [this](const dpp::confirmation_callback_t& cb) {
            if (cb.is_error()) {
                m_cluster->log(dpp::ll_error, "Failed to update commands: " + cb.http_info.body);
            } else {
                m_cluster->log(dpp::ll_info, "Slash commands synchronized successfully.");
            }
        });
    } else {
        m_cluster->log(dpp::ll_info, "Slash commands already up to date.");
    } });
}

void Client::OnReady(const dpp::ready_t &event)
{
    RegisterSlashCommands();
}

void Client::OnInteractionCreate(const dpp::slashcommand_t &event)
{
    auto cmdName = event.command.get_command_name();
    auto it = CommandRegistry.find(cmdName);

    if (it != CommandRegistry.end())
    {
        auto cmd = it->second(event);
        cmd->Execute();
    }
    else
    {
        event.reply("❌ Unknown command: " + cmdName);
    }
}

void Client::OnMessageCreate(const dpp::message_create_t &event)
{
    for (const auto &mention : event.msg.mentions)
    {
        if (mention.first.id == m_cluster->me.id)
        {
            dpp::embed embed;
            embed.set_title("Axion™ - Discord Rooster Minigame Bot");
            embed.set_description(m_i18n->GetString("messages.welcome_user"));
            embed.set_color(0x8e44ad);

            m_cluster->message_create(dpp::message(event.msg.channel_id, embed).set_reference(event.msg.id));
            break;
        }
    }
}
