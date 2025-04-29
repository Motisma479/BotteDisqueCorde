#include "Commands/Reload.hpp"
#include "advancedFunction.hpp"

Commands::Reload::Reload(const char* _name, dpp::cluster& bot, Data& data, std::vector<std::unique_ptr<Commands::ICommand>>& CommandList) : ICommand(_name, bot, data), CommandList(CommandList) {}

void Commands::Reload::Init(bool registerCommand, uint64_t _commandId)
{   
    ICommand::Init(registerCommand, _commandId);

    if (dpp::run_once<struct register_bot_commands>())
    {
        command = dpp::slashcommand(name, "reload the bot data.", cp_bot.me.id);
        command.set_default_permissions(dpp::p_administrator);
        dpp::command_option option_1(dpp::co_string, "command", "[superAdmin] force reset a specifique command ( not done by default ).", false);

        for (int i = 0; i < CommandList.size(); i++)
        {
            option_1.add_choice(dpp::command_option_choice(CommandList[i].get()->name, CommandList[i].get()->name));
        }
        command.add_option(option_1);

        if (registerCommand)
        {
            cp_bot.global_command_create(command);
        }
        else
        {
            command.id = _commandId;
            cp_bot.global_command_edit(command); //automaticaly reload the command to get the last command;
        }
    }
}

void Commands::Reload::Execute(const dpp::slashcommand_t& event)
{
    ICommand::Execute(event);
    if (event.command.get_command_name() == name) {
        bool asResetedCommand = false;
        std::string resetedCommand = "";

        auto start_time = std::chrono::high_resolution_clock::now();
        cp_data.Reload();

        asResetedCommand = std::holds_alternative<std::string>(event.get_parameter("command"));
        if(asResetedCommand)
        {
            if (!advanced::CheckSuperAdminID(event.command.get_issuing_user().id))
            {
                event.reply(dpp::message("Only super admin can hard reset a command.").set_flags(dpp::m_ephemeral));
                return;
            }

            resetedCommand = std::get<std::string>(event.get_parameter("command"));
            for (int i = 0; i < CommandList.size(); i++)
            {
                
                CommandList[i].get()->Reload(CommandList[i].get()->name == resetedCommand);
            }
        }
        else
            for (int i = 0; i < CommandList.size(); i++)
            {
                CommandList[i].get()->Reload(false);
            }
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();


        std::ostringstream response;
        response << ":white_check_mark: **Reload completed successfully!**\n>>> :clock3: Duration: " << duration_ms
            << " ms\n :repeat: Command Reloaded: " << CommandList.size()
            << "\n :moyai: Amogus file listed: " << cp_data.GetSusImages().size()
            << "\n :wine_glass: Meme file listed: " << cp_data.GetMemeImages().size();
        if (asResetedCommand)
        {
            response << "\n :pager: Command forced updated: " << resetedCommand;
        }

        dpp::message temp(response.str());
        temp.set_flags(dpp::m_ephemeral);
        event.reply(temp);
    }
}