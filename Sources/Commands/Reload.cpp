#include "Commands/Reload.hpp"

Commands::Reload::Reload(dpp::cluster& bot, Data& data, std::vector<std::unique_ptr<Commands::ICommand>>& CommandList) : ICommand(bot, data), CommandList(CommandList)
{
    name = "reload";
}

void Commands::Reload::Init(bool registerCommand)
{
    if (registerCommand && dpp::run_once<struct register_bot_commands>()) {
        dpp::slashcommand newcommand("reload", "reload the bot data.", cp_bot.me.id);
        newcommand.set_default_permissions(dpp::p_administrator);
        cp_bot.global_command_create(newcommand);
    }
}

void Commands::Reload::Execute(const dpp::slashcommand_t& event)
{
    if (event.command.get_command_name() == "reload") {
        auto start_time = std::chrono::high_resolution_clock::now();
        cp_data.Reload();
        for (int i = 0; i < CommandList.size(); i++)
        {
            CommandList[i].get()->Reload();
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();


        std::ostringstream response;
        response << ":white_check_mark: **Reload completed successfully!**\n> :clock3: Duration: " << duration_ms
            << " ms\n> :repeat: Command Reloaded: " << CommandList.size()
            << "\n> :moyai: Amogus file listed: " << cp_data.GetSusImages().size()
            << "\n> :wine_glass: Meme file listed: " << cp_data.GetMemeImages().size();
        dpp::message temp(response.str());
        temp.set_flags(dpp::m_ephemeral);
        event.reply(temp);
    }
}