#include "Commands/ListenToBanIp.hpp"

Commands::ListenToBanIp::ListenToBanIp(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data) {}

void Commands::ListenToBanIp::Init(bool registerCommand, uint64_t _commandId)
{
    ICommand::Init(registerCommand, _commandId);
    if (dpp::run_once<struct register_bot_commands>())
    {
        command = dpp::slashcommand(name, "Make the channel listen for IP bans on the server. If called a second time, it cancels the effect.", cp_bot.me.id);
        command.set_default_permissions(dpp::p_administrator);

        if (registerCommand)
        {
            cp_bot.global_command_create(command);
        }
    }
}

void Commands::ListenToBanIp::Execute(const dpp::slashcommand_t& event)
{
    ICommand::Execute(event);
    if (event.command.get_command_name() == name) {
        dpp::snowflake channelId = event.command.channel_id;

        if (cp_data.AddIpBanListner(channelId))
        {
            event.reply(dpp::message("<#" + std::to_string(channelId) + "> **is now __listening__ for IP bans on the server.**").set_flags(dpp::m_ephemeral));
            return;
        }

        event.reply(dpp::message("<#" + std::to_string(channelId) + "> **is __no longer__ listening for IP bans on the server.**").set_flags(dpp::m_ephemeral));
    }
}