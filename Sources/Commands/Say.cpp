#include "Commands/Say.hpp"

Commands::Say::Say(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data) {}

void Commands::Say::Init(bool registerCommand, uint64_t _commandId)
{
    if (registerCommand && dpp::run_once<struct register_bot_commands>()) {
        dpp::slashcommand newcommand(name, "make the bot say a message", cp_bot.me.id);
        newcommand.add_option( dpp::command_option(dpp::co_string, "message", "The me to send", true))
                  .add_option( dpp::command_option(dpp::co_string, "reply_id", "Reply to a message from the id", false))
                  .set_default_permissions(dpp::p_administrator);
        cp_bot.global_command_create(newcommand);
    }
}

void Commands::Say::Execute(const dpp::slashcommand_t& event)
{
    if (event.command.get_command_name() == name) {

        dpp::message message(std::get<std::string>(event.get_parameter("message")));
        message.channel_id = event.command.channel_id;
        
        //dpp::snowflake replyMessageId = std::get<dpp::snowflake>(event.get_parameter("reply_id"));
        //dpp::user invitated = event.command.get_resolved_user(invitated_id);
        auto reply_id_param = event.get_parameter("reply_id");
        if (reply_id_param.index() != 0)
        {
            std::string reply_id_str = std::get<std::string>(reply_id_param);
            dpp::snowflake reply_id(reply_id_str);
            message.message_reference.message_id = reply_id;

            event.reply(dpp::message("> **You replied to a message with: **_" + message.content + '_').set_flags(dpp::m_ephemeral));
        }
        else
        {
            event.reply(dpp::message("> **You send: **_" + message.content + '_').set_flags(dpp::m_ephemeral));
        }
        message.allowed_mentions.parse_users = true;
        message.allowed_mentions.parse_everyone = true;
        message.allowed_mentions.parse_roles = true;
        message.allowed_mentions.replied_user = true;
        cp_bot.message_create(message);
    }
}