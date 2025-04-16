#include "Commands/Stop.hpp"

Commands::Stop::Stop(dpp::cluster& bot, Data& data) : ICommand(bot, data)
{
    name = "stop";
}


void Commands::Stop::Init(bool registerCommand)
{
    if (registerCommand && dpp::run_once<struct register_bot_commands>()) {
        dpp::slashcommand newcommand("stop", "stop the bot.", cp_bot.me.id);
        newcommand.set_default_permissions(dpp::p_administrator);
        cp_bot.global_command_create(newcommand);
    }
}

void Commands::Stop::Execute(const dpp::slashcommand_t& event)
{
    if (event.command.get_command_name() == "stop") {
        dpp::message temp;
        temp.add_component(
            dpp::component()
            .add_component(
                dpp::component().
                set_emoji("button", 1361998188831047831).
                set_type(dpp::cot_button).
                set_style(dpp::cos_danger).
                set_id("stop")
            )
        );
        event.reply(temp.set_flags(dpp::m_ephemeral));
    }
}