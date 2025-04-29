#include "Commands/Stop.hpp"

Commands::Stop::Stop(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data) {}


void Commands::Stop::Init(bool registerCommand, uint64_t _commandId)
{
    ICommand::Init(registerCommand, _commandId);
    if (dpp::run_once<struct register_bot_commands>())
    {
        command = dpp::slashcommand(name, "stop the bot.", cp_bot.me.id);
        command.set_default_permissions(dpp::p_administrator);

        if (registerCommand)
        {
            cp_bot.global_command_create(command);
        }
    }
}

void Commands::Stop::Execute(const dpp::slashcommand_t& event)
{
    ICommand::Execute(event);
    if (event.command.get_command_name() == name) {
        dpp::message temp;
        temp.add_component(
            dpp::component()
            .add_component(
                dpp::component().
                set_emoji("powerButton", 1362438500661592224).
                set_type(dpp::cot_button).
                set_style(dpp::cos_danger).
                set_id("stop")
            )
        );
        event.reply(temp.set_flags(dpp::m_ephemeral));
    }
}