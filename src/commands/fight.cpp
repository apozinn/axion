#include "iostream"
#include <fmt/core.h>
#include "string"

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>

#include "commands/fight.hpp"
#include "databaseManager/databaseManager.hpp"
#include "rooster/utils.hpp"
#include "utils/utils.hpp"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using fmt::format;

void FightCommand::Execute()
{
    auto databaseConnecttion = DatabaseManager::Get().GetConnect();
    auto usersCollection = (*databaseConnecttion)["axion"]["users"];
    auto userDatabase = usersCollection.find_one(make_document(kvp("_id", std::to_string(m_author.id))));

    dpp::embed m_embed = MainEmbed();
    dpp::component m_buttons = MainButtons();
    dpp::message msg;
    msg.add_embed(m_embed);
    msg.add_component(m_buttons);
    m_event.reply(msg);
}

dpp::embed FightCommand::MainEmbed()
{
    dpp::embed embed = dpp::embed()
    	.set_author(m_client->me.username, "", m_client->me.get_avatar_url())
    	.set_thumbnail("https://imgur.com/RlMdIan.png")
    	.set_color(0xed0024)
    	.set_description(fmt::format("Treinador: {}(**{}**)\nRank: **{}^**/{}(**{}#**)\nPartidas: {}({}w / {}l)\nTítulo: {}",
    		m_author.username,
    		m_author.id,
    		std::to_string(0),
    		"Galo júnior",
    		std::to_string(100),
    		std::to_string(10),
    		std::to_string(7),
    		std::to_string(3),
    		"O corajoso"
    	))
    	.add_field(
    		"Galos disponíveis",
    		RoosterUtils::AvailableRooters(m_userRoosters).append("\n\nCom duvídas ou problemas? use **/sobre**")
    	)
    	.set_footer(dpp::embed_footer().set_text(m_author.username).set_icon(m_author.get_avatar_url()))
    	.set_timestamp(time(0));
    return embed;
}

dpp::component FightCommand::MainButtons()
{
    dpp::component component = dpp::component();
    component.add_component(
            Utils::GenerateButton("Galinheiro", dpp::cos_success, "rinha_galinheiro"))
        .add_component(
            Utils::GenerateButton("Treinamento", dpp::cos_success, "rinha_treinamento"))
        .add_component(
            Utils::GenerateButton("Batalha", dpp::cos_success, "rinha_batalha"));
    return component;
}