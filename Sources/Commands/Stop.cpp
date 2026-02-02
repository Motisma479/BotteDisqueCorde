#include "Commands/Stop.hpp"

Commands::Stop::Stop(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data) {}


void Commands::Stop::Init(CommandIds _commandIds)
{
    if (dpp::run_once<struct register_bot_commands>())
    {
        commands.chatCommand = new dpp::slashcommand(name, "stop the bot.", cp_bot.me.id);
        commands.chatCommand->set_default_permissions(dpp::p_administrator);

        ICommand::Init(_commandIds);
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