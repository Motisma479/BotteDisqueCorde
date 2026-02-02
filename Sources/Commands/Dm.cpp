#include "Commands/Dm.hpp"

Commands::Dm::Dm(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data) {}

void Commands::Dm::Init(CommandIds _commandIds)
{
    if (dpp::run_once<struct register_bot_commands>())
    {
        commands.chatCommand = new dpp::slashcommand(name, "send a dm trough the bot", cp_bot.me.id);
        commands.chatCommand->add_option(dpp::command_option(dpp::co_user, "user", "The user to send a message to", true))
            .add_option(dpp::command_option(dpp::co_string, "message", "the message to send", true))
            .set_default_permissions(dpp::p_administrator);
        
        commands.userCommand = new dpp::slashcommand(name, "send a dm trough the bot", cp_bot.me.id);
        commands.userCommand->set_default_permissions(dpp::p_administrator).set_type(dpp::ctxm_user);

        ICommand::Init(_commandIds);
    }
}

void Commands::Dm::Execute(const dpp::slashcommand_t& _event)
{
    ICommand::Execute(_event);
    if (_event.command.get_command_name() == name) {
        dpp::snowflake targetId =  std::get<dpp::snowflake>(_event.get_parameter("user"));
        dpp::message message(std::get<std::string>(_event.get_parameter("message")));
        message.channel_id = _event.command.channel_id;

        message.allowed_mentions.parse_users = true;
        message.allowed_mentions.parse_everyone = true;
        message.allowed_mentions.parse_roles = true;
        message.allowed_mentions.replied_user = true;

        _event.reply(dpp::message("**Sendind a DM...**").set_flags(dpp::m_ephemeral));

        dpp::message sucessAwnser = dpp::message("> **You send: **_" + message.content + "_\n> **To: ** <@" + std::to_string(targetId) + ">").set_flags(dpp::m_ephemeral);
        cp_bot.direct_message_create(targetId, message, [_event, sucessAwnser](const dpp::confirmation_callback_t& callback)
        {
             if(!callback.is_error())
             {
                 _event.edit_original_response(sucessAwnser);
             }
             else
             {
                 _event.edit_original_response(dpp::message("**ERROR: " + callback.get_error().message + "**").set_flags(dpp::m_ephemeral));
             }
        });
    }
}

void  Commands::Dm::Execute(const dpp::user_context_menu_t& _event)
{
    ICommand::Execute(_event);

    if (_event.command.get_command_name() == name)
    {
        dpp::interaction_modal_response modal("dm_modal_" + _event.get_user().id.str(), "Please write your message:");

        /* Add a text component */
        modal.add_component(
            dpp::component()
            .set_type(dpp::cot_text)
            .set_text_style(dpp::text_paragraph)
            .set_label("message")
            .set_id("message")
            .set_placeholder("A nice message.")
        );

        _event.dialog(modal);
    }
}
