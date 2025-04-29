#include "Commands/Pressence.hpp"

Commands::Pressence::Pressence(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data) {}

void Commands::Pressence::Init(bool registerCommand, uint64_t _commandId)
{
    ICommand::Init(registerCommand, _commandId);
    if (dpp::run_once<struct register_bot_commands>())
    {
        command = dpp::slashcommand(name, "Change the pressence of the bot.", cp_bot.me.id);
        command.add_option(dpp::command_option(dpp::co_string, "message", "The pressence message.", false));
        if (registerCommand)
        {
            cp_bot.global_command_create(command);
        }
    }
}

void Commands::Pressence::Execute(const dpp::slashcommand_t& event)
{
    ICommand::Execute(event);
    if (event.command.get_command_name() == name) {
        if (std::holds_alternative<std::string>(event.get_parameter("message")))
        {
            std::string m = std::get<std::string>(event.get_parameter("message"));
            cp_data.SetPressenceMessage(m);
            cp_bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_custom, m));
        }
        else
        {
            cp_data.SetPressenceMessage("");
            cp_bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_custom, ""));
        }
        event.reply(dpp::message("> **:white_check_mark: Bot pressence updated.**").set_flags(dpp::m_ephemeral));
    }
}