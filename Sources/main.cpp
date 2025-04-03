#include <iostream>
#include <chrono>
#include <ctime>  
#include <memory>

#include "advancedFunction.hpp"
#include "Commands/Commands.hpp"

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
int main()
{

    CommandList.push_back(std::make_unique<Commands::Amogus>(bot, data));
    CommandList.push_back(std::make_unique<Commands::Clear>(bot, data));
    CommandList.push_back(std::make_unique<Commands::Dice>(bot, data));
    CommandList.push_back(std::make_unique<Commands::Invite>(bot, data));
    CommandList.push_back(std::make_unique<Commands::Meme>(bot, data));
    CommandList.push_back(std::make_unique<Commands::Ping>(bot, data));
    CommandList.push_back(std::make_unique<Commands::Reload>(bot, data, CommandList));
    CommandList.push_back(std::make_unique<Commands::Stop>(bot, data));

    bot.on_log(dpp::utility::cout_logger());
    
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        for (int i = 0; i < CommandList.size(); i++)
        {
            CommandList[i].get()->Execute(event);
        }
    });

    bot.on_ready([](const dpp::ready_t& event) {
        for (int i = 0; i < CommandList.size(); i++)
        {
            CommandList[i].get()->Init();
        }
    });

    /* Handle button action */
    bot.on_button_click([&](const dpp::button_click_t& event) {
        //stop ✔
        if(event.custom_id == "stop" && advanced::CheckSuperAdminID(event.command.usr.id))
        {
            bot.message_delete(event.command.message_id, event.command.channel_id);
            if(data.GetStopMachine())
                system(SHUTDOWN_COMMAND);
            Sleep(500);
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
    bot.start(static_cast<dpp::start_type>(false));
    return 0;
}
