#include "Commands/Dm.hpp"

Commands::Dm::Dm(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data) {}

void Commands::Dm::Init(bool registerCommand, uint64_t _commandId)
{
    if (registerCommand && dpp::run_once<struct register_bot_commands>()) {
        dpp::slashcommand newcommand(name, "make the bot send a dm", cp_bot.me.id);
        newcommand.add_option(dpp::command_option(dpp::co_user, "user", "The user to send a message to", true))
            .add_option(dpp::command_option(dpp::co_string, "message", "the message to send", true))
            .set_default_permissions(dpp::p_administrator);
        cp_bot.global_command_create(newcommand);
    }
}

void Commands::Dm::Execute(const dpp::slashcommand_t& event)
{
    if (event.command.get_command_name() == name) {
        dpp::snowflake targetId =  std::get<dpp::snowflake>(event.get_parameter("user"));
        dpp::message message(std::get<std::string>(event.get_parameter("message")));
        message.channel_id = event.command.channel_id;


        message.allowed_mentions.parse_users = true;
        message.allowed_mentions.parse_everyone = true;
        message.allowed_mentions.parse_roles = true;
        message.allowed_mentions.replied_user = true;

        event.reply(dpp::message("**Sendind a DM...**").set_flags(dpp::m_ephemeral));

        dpp::message sucessAwnser = dpp::message("> **You send: **_" + message.content + "_\n> **To: ** <@" + std::to_string(targetId) + ">").set_flags(dpp::m_ephemeral);
        cp_bot.direct_message_create(targetId, message, [event, sucessAwnser](const dpp::confirmation_callback_t& callback)
        {
             if(!callback.is_error())
             {
                 event.edit_original_response(sucessAwnser);
             }
             else
             {
                 event.edit_original_response(dpp::message("**ERROR: " + callback.get_error().message + "**").set_flags(dpp::m_ephemeral));
             }
        });
    }
}