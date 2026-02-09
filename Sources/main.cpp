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
    CommandList.push_back(std::make_unique<Commands::Amogus>("amogus", bot, data));
    CommandList.push_back(std::make_unique<Commands::Clear>("clear", bot, data));
    CommandList.push_back(std::make_unique<Commands::Dice>("dice", bot, data));
    CommandList.push_back(std::make_unique<Commands::Dm>("dm", bot, data));
    CommandList.push_back(std::make_unique<Commands::Guess>("guess", bot, data));
    CommandList.push_back(std::make_unique<Commands::Invite>("invite", bot, data));
    CommandList.push_back(std::make_unique<Commands::ListenToBanIp>("listen_to_ban_ip", bot, data));
    CommandList.push_back(std::make_unique<Commands::Meme>("meme", bot, data));
    CommandList.push_back(std::make_unique<Commands::Ping>("ping", bot, data));
    CommandList.push_back(std::make_unique<Commands::Poll>("poll", bot, data, pollManager));
    CommandList.push_back(std::make_unique<Commands::Pressence>("pressence", bot, data));
    CommandList.push_back(std::make_unique<Commands::Reload>("reload", bot, data, CommandList));
    CommandList.push_back(std::make_unique<Commands::Say>("say", bot, data));
    CommandList.push_back(std::make_unique<Commands::Settings>("settings", bot, data));
    CommandList.push_back(std::make_unique<Commands::Status>("status", bot, data));
    CommandList.push_back(std::make_unique<Commands::Stop>("stop", bot, data));
    CommandList.push_back(std::make_unique<Commands::SuperAdmin>("super_admin", bot, data));
    CommandList.push_back(std::make_unique<Commands::UnbanIp>("unban_ip", bot, data));
    CommandList.push_back(std::make_unique<Commands::Upload>("upload", bot, data));
    CommandList.push_back(std::make_unique<Commands::Weather>("weather", bot, data));

    bot.on_log(dpp::utility::cout_logger());
    
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        for (int i = 0; i < CommandList.size(); i++)
        {
            CommandList[i].get()->Execute(event);
        }
    });

    bot.on_message_context_menu([](const dpp::message_context_menu_t& event) {
        for (int i = 0; i < CommandList.size(); i++)
        {
            CommandList[i].get()->Execute(event);
        }
    });

    bot.on_user_context_menu([](const dpp::user_context_menu_t& event) {
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
        bot.start_timer([&](const dpp::timer&) {
            std::cout << "making a weather request\n";
            std::string weatherAPILink = "https://api.openweathermap.org/data/3.0/onecall?lat=" + std::to_string(data.GetLatitude()) + "&lon=" + std::to_string(data.GetLongitude()) + "&exclude=minutely,hourly,daily,alerts&units=" + data.GetMetrics() + "&appid=" + GetTokenWeather();
            bot.request(weatherAPILink, dpp::m_get,
                [](const dpp::http_request_completion_t& result) {
                    if (result.status == 200)
                    {
                        dpp::json response = dpp::json::parse(result.body);
                        data.SetWeatherData({
                            response["current"]["temp"],
                            response["current"]["feels_like"],
                            response["current"]["humidity"],
                            response["current"]["clouds"],
                            response["current"]["uvi"],
                            response["current"]["wind_speed"],
                            response["current"]["wind_deg"],
                            response["current"]["weather"][0]["id"],
                            response["current"]["weather"][0]["main"],
                            response["current"]["weather"][0]["description"]
                            });
                    }
                });
        }, 600);
        {
            std::cout << "making a weather request\n";
            std::string weatherAPILink = "https://api.openweathermap.org/data/3.0/onecall?lat=" + std::to_string(data.GetLatitude()) + "&lon=" + std::to_string(data.GetLongitude()) + "&exclude=minutely,hourly,daily,alerts&units=" + data.GetMetrics() + "&appid=" + GetTokenWeather();
            bot.request(weatherAPILink, dpp::m_get,
                [](const dpp::http_request_completion_t& result) {
                    if (result.status == 200)
                    {
                        dpp::json response = dpp::json::parse(result.body);
                        data.SetWeatherData({
                            response["current"]["temp"],
                            response["current"]["feels_like"],
                            response["current"]["humidity"],
                            response["current"]["clouds"],
                            response["current"]["uvi"],
                            response["current"]["wind_speed"],
                            response["current"]["wind_deg"],
                            response["current"]["weather"][0]["id"],
                            response["current"]["weather"][0]["main"],
                            response["current"]["weather"][0]["description"]
                            });
                    }
                });
        }

        bot.global_commands_get([&](const dpp::confirmation_callback_t& callback) {
            std::vector<std::pair<std::string, CommandIds>> existingCommand;

            if (!callback.is_error()) {
                auto commands = std::get<dpp::slashcommand_map>(callback.value);
                for (const auto& [id, cmd] : commands) {
                    std::cout << "Existing Command: " << cmd.name << " (ID: " << id << ")\n";

                    std::vector<std::pair<std::string, CommandIds>>::iterator it;
                    it = std::find_if(existingCommand.begin(), existingCommand.end(), [&](const std::pair<std::string, CommandIds>& entry) {return entry.first == cmd.name; });
                    if (it != existingCommand.end())
                    {
                        if(cmd.type == dpp::ctxm_chat_input)
                            it->second.chat = id;
                        else if (cmd.type == dpp::ctxm_message)
                            it->second.message = id;
                        else if (cmd.type == dpp::ctxm_user)
                            it->second.user = id;
                    }
                    else
                    {
                        CommandIds temp;
                        if (cmd.type == dpp::ctxm_chat_input)
                            temp.chat = id;
                        else if (cmd.type == dpp::ctxm_message)
                            temp.message = id;
                        else if (cmd.type == dpp::ctxm_user)
                            temp.user = id;
                        existingCommand.push_back(std::make_pair(cmd.name,temp));
                    }
                        
                }
            }

            for (auto& command : CommandList)
            {
                std::vector<std::pair<std::string, CommandIds>>::iterator it;
                it = std::find_if(existingCommand.begin(), existingCommand.end(), [&](const std::pair<std::string, CommandIds>& entry){return entry.first == command->name;});
                if (it != existingCommand.end())
                {
                    command->Init(it->second);
                    existingCommand.erase(it);
                }
                else
                    command->Init({0,0,0});
            }

            for(const auto& [name, ids] : existingCommand)//delete all the obselet command
            {
                if(ids.chat != 0)
                    bot.global_command_delete(ids.chat);

                if (ids.message != 0)
                    bot.global_command_delete(ids.message);

                if (ids.user != 0)
                    bot.global_command_delete(ids.user);
                
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
        if (event.custom_id == "star_newcommand")
        {
            dpp::interaction_modal_response modal("star_commandmodal", "Please enter the command");

            ///* Add a text component */
            modal.add_component(
                dpp::component()
                .set_label("Command:")
                .set_id("field_command")
                .set_type(dpp::cot_text)
                .set_placeholder("cd ~")
                .set_min_length(1)
                .set_max_length(4000)
                .set_text_style(dpp::text_short)
            );


            //
            //
            ///* Trigger the dialog box. All dialog boxes are ephemeral */
            event.dialog(modal);
        }
    });

    bot.on_form_submit([](const dpp::form_submit_t& _event) {
        if (_event.custom_id == "settings_modal")
        {
            data.SetStopMachine(std::get<std::string>(_event.components[0].value) == "true");
            data.SetLatitude(std::stod(std::get<std::string>(_event.components[2].value)));
            data.SetLongitude(std::stod(std::get<std::string>(_event.components[3].value)));
            data.SetMetrics(std::get<std::string>(_event.components[4].value));

            data.SetPressenceMessage(std::get<std::string>(_event.components[1].value)); //set the pressence message at last since it save the settings
            bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_custom, std::get<std::string>(_event.components[1].value)));

            _event.reply(dpp::message("**New Settings saved!**").set_flags(dpp::m_ephemeral));
        }
        if (_event.custom_id.starts_with("dm_modal_"))
        {
            std::string message_text = std::get<std::string>(_event.components[0].components[0].value);
            dpp::snowflake targetId(_event.custom_id.substr(std::string("dm_modal_").length()));


            dpp::message message(message_text);
            message.allowed_mentions.parse_users = true;
            message.allowed_mentions.parse_everyone = true;
            message.allowed_mentions.parse_roles = true;
            message.allowed_mentions.replied_user = true;
            
            _event.reply(dpp::message("**Sendind a DM...**").set_flags(dpp::m_ephemeral));
            dpp::message sucessAwnser = dpp::message("> **You send: **_" + message.content + "_\n> **To: ** <@" + std::to_string(targetId) + ">").set_flags(dpp::m_ephemeral);
            
            bot.direct_message_create(targetId, message, [_event, sucessAwnser](const dpp::confirmation_callback_t& callback)
            {
                if (!callback.is_error())
                {
                    _event.edit_original_response(sucessAwnser);
                }
                else
                {
                    _event.edit_original_response(dpp::message("**ERROR: " + callback.get_error().message + "**").set_flags(dpp::m_ephemeral));
                }
            });
        }
    });

    /* Start the bot */    
    bot.start(dpp::st_wait);
    return 0;
}
