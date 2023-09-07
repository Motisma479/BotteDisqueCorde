#include "Commands/Reload.hpp"

Commands::Reload::Reload(dpp::cluster& bot, Data& data, std::vector<std::unique_ptr<Commands::ICommand>>& CommandList) : ICommand(bot, data), CommandList(CommandList)
{}

void Commands::Reload::Init()
{
    if (dpp::run_once<struct register_bot_commands>()) {
        dpp::slashcommand newcommand("reload", "reload the bot data.", cp_bot.me.id);
        newcommand.set_default_permissions(dpp::p_administrator);
        cp_bot.global_command_create(newcommand);
    }
}

void Commands::Reload::Execute(const dpp::slashcommand_t& event)
{
    if (event.command.get_command_name() == "reload") {
        cp_data.Reload();
        for (int i = 0; i < CommandList.size(); i++)
        {
            CommandList[i].get()->Reload();
        }
        event.reply("sucess");
    }
}