#include <iostream>
#include <chrono>
#include <ctime>  
#include <random>

#include "Initialiser.hpp"
#include "advancedFunction.hpp"

#ifdef _WIN32
#define SHUTDOWN_COMMAND "shutdown -t 0 -f -s"
#elif __unix || __unix__ || __linux__
#define SHUTDOWN_COMMAND "poweroff"
#elif __APPLE__ || __MACH__
//TODO
#endif
//✔❌
#include <dpp/dpp.h>

const std::string    BOT_TOKEN = getToken();
dpp::cluster bot(BOT_TOKEN);

std::mt19937 rng;
std::uniform_int_distribution<int> generator;

void callback(const dpp::confirmation_callback_t& event)
{
    if (event.is_error())
        return;
    std::vector<dpp::snowflake> msgids;
    dpp::message_map object = std::get<dpp::message_map>(event.value);
    dpp::snowflake chanelId = object.begin()->second.channel_id;
    for (auto m = object.begin(); m != object.end(); m++)
        //bot.message_delete(m->second.id, m->second.channel_id);
        msgids.push_back(m->second.id);

    bot.message_delete_bulk(msgids, chanelId);
    return;
}

Data data;
int main()
{
    generator = std::uniform_int_distribution<int>(0, data.getMaxSusImages() - 1);
    rng.seed(time(NULL));

    static int lastRandom = -1;
    /* Create bot cluster */

    /* Output simple log messages to stdout */
    bot.on_log(dpp::utility::cout_logger());
    
    /* Handle slash command */
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        // /ping ✔
        if (event.command.get_command_name() == "ping") {
            double APIH = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
            APIH /= 1000;
            double result = (APIH - event.command.get_creation_time()) * 1000;
            std::string temp = "🏓Pong!\n------------------------\nlatency:\n" + std::to_string(abs(result)) + " ms" + "\n------------------------\nAPI:\n"+std::to_string(APIH - std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count()/1000) + " ms";
            event.reply(temp);
            
        }
        // /amogus ✔
        if (event.command.get_command_name() == "amogus") {
            dpp::message temp;

            int index;
            if (data.getMaxSusImages() > 1)
            {
                index = generator(rng);
                while (index == lastRandom) index = generator(rng);
                lastRandom = index;
            }
            else index = 0;
            char ImagePath[25] = "Resources/AMOGUS/XXX.png";
            ImagePath[17] = ((index / 100) % 10) + '0';
            ImagePath[18] = ((index / 10) % 10) + '0';
            ImagePath[19] = (index % 10) + '0';
            temp.add_file("AMOGUS.png", dpp::utility::read_file(ImagePath));

            event.reply(temp);
        }
        // /dice ✔
        if (event.command.get_command_name() == "dice") {
            int value;
            if (event.get_parameter("face").index() <= 0)
                value = 6;
            else
            {
                value = std::get<int64_t>(event.get_parameter("face"));
                if (value <= 0)
                    value = 1;
            }
            std::uniform_int_distribution<int> diceGenerator(1, value);
            std::string temp = std::to_string(diceGenerator(rng));
            event.reply(temp);
        }
        // /stop ✔
        if (event.command.get_command_name() == "stop") {
            dpp::message temp;
            temp.add_component(
                dpp::component()
                .add_component(
                    dpp::component().set_emoji("✔").
                    set_type(dpp::cot_button).
                    set_style(dpp::cos_secondary).
                    set_id("stop")
                ).add_component(
                    dpp::component().set_emoji("❌").
                    set_type(dpp::cot_button).
                    set_style(dpp::cos_secondary).
                    set_id("del")
                )
            );
            event.reply(temp);
        }
        // /clear ✔
        if (event.command.get_command_name() == "clear") {
            int value;
            if (event.get_parameter("count").index() <= 0)
                value = 10;
            else
            {
                value = std::get<int64_t>(event.get_parameter("count"));
                if (value <= 0)
                    value = 2;
            }
            bot.messages_get(event.command.channel_id, 0, event.command.message_id, 0, value, callback);
            event.reply("complete");
            event.delete_original_response();
        }
        //invite game player
        if (event.command.get_command_name() == "invite") {

                std::string game = std::get<std::string>(event.get_parameter("game"));
                dpp::snowflake invitated_id = std::get<dpp::snowflake>(event.get_parameter("user"));
                dpp::user invitated = event.command.get_resolved_user(invitated_id);

                dpp::embed embed = dpp::embed().
                set_color(0xAA8ED6).
                set_author(invitated.username, "", invitated.get_avatar_url()).
                set_description(invitated.get_mention() + "\n———VS———\n" + event.command.member.get_mention() ).
                set_thumbnail("https://dpp.dev/DPP-Logo.png").
                add_field(
                    "Do you accept ?",
                    "———————"
                ).
                set_footer(dpp::embed_footer().set_text("Invited by "+ event.command.usr.username).set_icon(event.command.usr.get_avatar_url())).
                set_timestamp(time(0));
                event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
                dpp::message temp(event.command.channel_id,"");
                temp.add_component(
                    dpp::component()
                    .add_component(
                        dpp::component().set_emoji("✔").
                        set_type(dpp::cot_button).
                        set_style(dpp::cos_secondary).
                        set_id("dal")
                    ).add_component(
                        dpp::component().set_emoji("❌").
                        set_type(dpp::cot_button).
                        set_style(dpp::cos_secondary).
                        set_id("del")
                    )
                );
                bot.message_create(temp);
        }
        });
    /*
     dpp::embed embed = dpp::embed().
                set_color(0xAA8ED6).
                set_author("Some name", "https://dpp.dev/", "https://dpp.dev/DPP-Logo.png").
                set_description("Some description here").
                set_thumbnail("https://dpp.dev/DPP-Logo.png").
                add_field(
                    "Regular field title",
                    "Some value here"
                ).
                add_field(
                    "Inline field title",
                    "Some value here",
                    true
                ).
                add_field(
                    "Inline field title",
                    "Some value here",
                    true
                ).
                set_image("https://dpp.dev/DPP-Logo.png").
                set_footer(dpp::embed_footer().set_text("Some footer text here").set_icon("https://dpp.dev/DPP-Logo.png")).
                set_timestamp(time(0));
                event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id)); 
    */
    
    /* Register slash command here in on_ready */
    bot.on_ready([](const dpp::ready_t& event) {
        /* Wrap command registration in run_once to make sure it doesnt run on every full reconnection */
        // /ping
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));
        } 
        // /amogus
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("amogus", "did you say AMOGUS?", bot.me.id));
        }
        // /dice
        if (dpp::run_once<struct register_bot_commands>()) {
            dpp::slashcommand newcommand("dice", "roll a dice", bot.me.id);
            newcommand.add_option(
                dpp::command_option(dpp::co_integer, "face", "the number face", false)
            );
            bot.global_command_create(newcommand);
        }
        // /stop
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("stop", "stop the bot", bot.me.id));
        }
        // /clear
        if (dpp::run_once<struct register_bot_commands>()) {
            dpp::slashcommand newcommand("clear", "clear the chanel", bot.me.id);
            newcommand.add_option(
                dpp::command_option(dpp::co_integer, "count", "the number of message to delete", false)
            );
            bot.global_command_create(newcommand);
        }
        //invite game player
        if (dpp::run_once<struct register_bot_commands>()) {
            dpp::slashcommand newcommand("invite", "invite a user to play some game", bot.me.id);
            newcommand.add_option(
                dpp::command_option(dpp::co_string, "game", "The game to play", true).
                    add_choice(dpp::command_option_choice("chess", std::string("game_chess")))
            ).add_option(
                dpp::command_option(dpp::co_user, "user", "User to play with", true)
            );
            bot.global_command_create(newcommand);
        }
        });

    /* Handle button action */
    bot.on_button_click([](const dpp::button_click_t& event) {
        //stop ✔
        if(event.custom_id == "stop" && advanced::CheckSuperAdminID(event.command.usr.id))
        {
            bot.message_delete(event.command.message_id, event.command.channel_id);
            system(SHUTDOWN_COMMAND);
        }
        //del ✔
        if (event.custom_id == "del")
        {
            bot.message_delete(event.command.message_id, event.command.channel_id);
        }
    });

    /* Start the bot */
    bot.start(false);
    return 0;
}
