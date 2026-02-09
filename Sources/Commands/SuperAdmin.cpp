#include "Commands/SuperAdmin.hpp"
#include "advancedFunction.hpp"

Commands::SuperAdmin::SuperAdmin(const char* _name, dpp::cluster & bot, Data& data) : ICommand(_name, bot, data)
{
    name = "super_admin";
}

void Commands::SuperAdmin::Init(CommandIds _commandIds)
{
    if (dpp::run_once<struct register_bot_commands>())
    {
        commands.chatCommand = new dpp::slashcommand("super_admin", "Super Admin management!", cp_bot.me.id);
        commands.chatCommand->set_default_permissions(dpp::p_administrator).
            add_option(dpp::command_option(dpp::co_sub_command, "add", "Add a new superAdmin").add_option(dpp::command_option(dpp::co_user, "user", "the user to promote",true))).
            add_option(dpp::command_option(dpp::co_sub_command, "list", "List all the superAdmin")).
            add_option(dpp::command_option(dpp::co_sub_command, "delete", "Delete a superAdmin").add_option(dpp::command_option(dpp::co_user, "user", "the user to delete", true)));
         
        ICommand::Init(_commandIds);
    }
}

void Commands::SuperAdmin::Execute(const dpp::slashcommand_t& event)
{
    ICommand::Execute(event);
    if (event.command.get_command_name() == "super_admin") {
        const std::string subCommand = event.command.get_command_interaction().options[0].name;
        
        if (!advanced::CheckSuperAdminID(event.command.get_issuing_user().id))
        {
            event.reply(dpp::message("Only super admin can use that command.").set_flags(dpp::m_ephemeral));
            return;
        }

        std::vector<dpp::snowflake> ids;
        std::ifstream AdmIds;
        AdmIds.open("superAdminId");
        for (std::string line; std::getline(AdmIds, line); )
        {
            if (line.size() <= 1) continue;
            else
            {
                ids.push_back(std::stoull(line));
            }
        }
        AdmIds.close();

        if (subCommand == "add")
        {
            dpp::snowflake newUserId = std::get<dpp::snowflake>(event.get_parameter("user"));
            auto it = std::find(ids.begin(), ids.end(), newUserId);

            if (it != ids.end())
            {
                event.reply(dpp::message("> <@"+std::to_string(newUserId) + "> is already a super admin.").set_flags(dpp::m_ephemeral));
            }
            else
            {
                ids.push_back(newUserId);
                std::ofstream s;
                s.open("superAdminId");
                if (s.is_open())
                {
                    for (const dpp::snowflake& id : ids) s << std::to_string(id) << '\n';
                }
                event.reply(dpp::message("> <@" + std::to_string(newUserId) + "> is now a super admin.").set_flags(dpp::m_ephemeral));
            }
        }
        else if(subCommand == "list")
        {
            std::string message = "## SuperAdmin List:\n>>> ";
            for (const dpp::snowflake& id : ids)
            {
                message += "<@" + std::to_string(id) + ">\n";
            }
            event.reply(dpp::message(message).set_flags(dpp::m_ephemeral));
        }
        else if (subCommand == "delete")
        {
            dpp::snowflake newUserId = std::get<dpp::snowflake>(event.get_parameter("user"));
            if (newUserId == ids[0]) // add a security to not delete the main super admin
            {
                event.reply(dpp::message("> <@" + newUserId.str() + "> cannot be removed as he is the first super admin.").set_flags(dpp::m_ephemeral));
                return;
            }
            auto it = std::find(ids.begin(), ids.end(), newUserId);

            if (it != ids.end())
            {
                ids.erase(it);
                std::ofstream s;
                s.open("superAdminId");
                if (s.is_open())
                {
                    for (const dpp::snowflake& id : ids) s << std::to_string(id) << '\n';
                }
                event.reply(dpp::message("> <@" + std::to_string(newUserId) + "> is no longer a super admin.").set_flags(dpp::m_ephemeral));
            }
            else
            {
                event.reply(dpp::message("> <@" + std::to_string(newUserId) + "> is not a super admin.").set_flags(dpp::m_ephemeral));
            }
        }
    }
}