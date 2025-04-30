#include <iostream>
#include <chrono>
#include <ctime>  
#include <memory>
#include <thread>

#include "advancedFunction.hpp"
#include "Commands/Commands.hpp"
#include "MonitorIpBan.hpp"

#ifdef _WIN32
#define SHUTDOWN_COMMAND "shutdown -t 0 -f -s"
#elif __unix || __unix__ || __linux__
#define SHUTDOWN_COMMAND "poweroff"
#elif __APPLE__ || __MACH__
//TODO
#endif
//✔❌

const std::string BOT_TOKEN = GetToken();
dpp::cluster bot(BOT_TOKEN);

std::vector<std::unique_ptr<Commands::ICommand>> CommandList;

Data data;
PollManager pollManager(bot);
MonitorIpBan monitorIpBan(bot, data);
int main()
{
    monitorIpBan.Init();
    CommandList.push_back(std::make_unique<Commands::Amogus>("amogus", bot, data));
    CommandList.push_back(std::make_unique<Commands::Clear>("clear", bot, data));
    CommandList.push_back(std::make_unique<Commands::Dice>("dice", bot, data));
    CommandList.push_back(std::make_unique<Commands::Dm>("dm", bot, data));
    CommandList.push_back(std::make_unique<Commands::Invite>("invite", bot, data));
    CommandList.push_back(std::make_unique<Commands::ListenToBanIp>("listen_to_ban_ip", bot, data));
    CommandList.push_back(std::make_unique<Commands::Meme>("meme", bot, data));
    CommandList.push_back(std::make_unique<Commands::Ping>("ping", bot, data));
    CommandList.push_back(std::make_unique<Commands::Poll>("poll", bot, data, pollManager));
    CommandList.push_back(std::make_unique<Commands::Pressence>("pressence", bot, data));
    CommandList.push_back(std::make_unique<Commands::Reload>("reload", bot, data, CommandList));
    CommandList.push_back(std::make_unique<Commands::Say>("say", bot, data));
    CommandList.push_back(std::make_unique<Commands::Stop>("stop", bot, data));
    CommandList.push_back(std::make_unique<Commands::SuperAdmin>("super_admin", bot, data));

    bot.on_log(dpp::utility::cout_logger());
    
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        for (int i = 0; i < CommandList.size(); i++)
        {
            CommandList[i].get()->Execute(event);
        }
    });

    bot.on_ready([](const dpp::ready_t& event) {
        

        bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_custom, data.GetPressenceMessage()));//set the presence when bot is starting.
        bot.start_timer([&](dpp::timer) //The main loop but manage by the timer.
        {
            pollManager.Update();
            monitorIpBan.Monitor();
        }, 1);

        bot.global_commands_get([&](const dpp::confirmation_callback_t& callback) {
            std::vector<std::pair<std::string,uint64_t>> existingCommand;

            if (!callback.is_error()) {
                auto commands = std::get<dpp::slashcommand_map>(callback.value);
                for (const auto& [id, cmd] : commands) {
                    std::cout << "Existing Command: " << cmd.name << " (ID: " << id << ")\n";
                    existingCommand.push_back(std::make_pair(cmd.name,id));
                }
            }

            for (auto& command : CommandList)
            {
                std::vector<std::pair<std::string, uint64_t>>::iterator it;
                it = std::find_if(existingCommand.begin(), existingCommand.end(), [&](const std::pair<std::string, uint64_t>& entry){return entry.first == command->name;});
                if (it != existingCommand.end())
                {
                    command->Init(false,it->second);
                    existingCommand.erase(it);
                }
                else
                    command->Init(true, 0);
            }

            for(const auto& [name, id] : existingCommand)//delete all the obselet command
            {
                bot.global_command_delete(id);
            }
        });
    });

    /* Handle button action */
    bot.on_button_click([&](const dpp::button_click_t& event) {
        //stop ✔
        if(event.custom_id == "stop" && advanced::CheckSuperAdminID(event.command.usr.id))
        {
            std::string messageContent;
            if (data.GetStopMachine()) messageContent = "\n<:power:1362438577451172110> ** Hosting machine is sutting down ** <:power:1362438577451172110>\n";
            else messageContent = "\n<:power:1362438577451172110> ** Bot is sutting down ** <:power:1362438577451172110>\n";
            

            event.reply(dpp::interaction_response_type::ir_update_message, dpp::message(messageContent).set_flags(dpp::m_ephemeral));
	    std::this_thread::sleep_for(std::chrono::seconds(2));
            
                
            if (data.GetStopMachine())
                system(SHUTDOWN_COMMAND);
            bot.shutdown();
         
        }
        //del ✔
        if (event.custom_id == "del")
        {
            //do not work may need a rework.
            //auto a = event.command.msg;
            //event.edit_response(dpp::message("command canceled...").set_flags(dpp::m_ephemeral));
            //bot.message_delete(event.command.message_id, event.command.channel_id);
        }
    });

    /* Start the bot */    
    bot.start(dpp::st_wait);
    return 0;
}
