// BotteDisqueCorde.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>  
#include <random>

#ifdef _WIN32
#define SHUTDOWN_COMMAND "shutdown -t 0 -f -s"
#elif __unix || __unix__ || __linux__
#define SHUTDOWN_COMMAND "poweroff"
#elif __APPLE__ || __MACH__
//TODO
#endif





/*
int main()
{
    std::cout << "Hello World!\n";
}
*/
// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.


#include <dpp/dpp.h>

/* Be sure to place your token in the line below.
 * Follow steps here to get a token:
 * https://dpp.dev/creating-a-bot-application.html
 * When you invite the bot, be sure to invite it with the
 * scopes 'bot' and 'applications.commands', e.g.
 * https://discord.com/oauth2/authorize?client_id=940762342495518720&scope=bot+applications.commands&permissions=139586816064
 */
const std::string    BOT_TOKEN = "OTQ3MjMwMjM2MzUxODExNjE0.G7n4Io.WRMdYnKBvcVVxPbO9OhMyM6VpMuf4V2-8pM0yg";

#define MAX_SUS_IMAGES 62

std::mt19937 rng;
std::uniform_int_distribution<int> generator(0, MAX_SUS_IMAGES - 1);

int main()
{
bool stp = false;
    rng.seed(time(NULL));

    static int lastRandom = -1;
    /* Create bot cluster */
    dpp::cluster bot(BOT_TOKEN);

    /* Output simple log messages to stdout */
    bot.on_log(dpp::utility::cout_logger());

    /* Handle slash command */
    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping") {
            double APIH = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
            APIH /= 1000;
            double result = (APIH - event.command.get_creation_time()) * 1000;
            std::string temp = "🏓Pong!\n------------------------\nlatency:\n" + std::to_string(abs(result)) + " ms" + "\n------------------------\nAPI:\n"+std::to_string(APIH - std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count()/1000) + " ms";
            event.reply(temp);
            
        }
        if (event.command.get_command_name() == "amogus") {
            dpp::message temp;

            int index;
            if (MAX_SUS_IMAGES > 1)
            {
                index = generator(rng);
                while (index == lastRandom) index = generator(rng);
            }
            else index = 0;
            char ImagePath[25] = "Resources/AMOGUS/XXX.png";
            ImagePath[17] = ((index / 100) % 10) + '0';
            ImagePath[18] = ((index / 10) % 10) + '0';
            ImagePath[19] = (index % 10) + '0';
            temp.add_file("AMOGUS.png", dpp::utility::read_file(ImagePath));

            event.reply(temp);
        }
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
        if (event.command.get_command_name() == "clear") {
            dpp::snowflake temp = event.command.message_id;
            
            bot.message_delete(event.command.get_channel().last_message_id, event.command.channel_id);
            dpp::get_channel_cache
        });
    
    /* Register slash command here in on_ready */
    bot.on_ready([&bot](const dpp::ready_t& event) {
        /* Wrap command registration in run_once to make sure it doesnt run on every full reconnection */
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));
        } 
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("amogus", "did you say AMOGUS?", bot.me.id));
        }
        if (dpp::run_once<struct register_bot_commands>()) {
            dpp::slashcommand newcommand("dice", "roll a dice", bot.me.id);
            newcommand.add_option(
                dpp::command_option(dpp::co_integer, "face", "the number face", false)
            );
            bot.global_command_create(newcommand);
        }
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("stop", "stop the bot", bot.me.id));
        }
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("clear", "clear the chanel", bot.me.id));
        }
        });

    /* Handle button action */
    bot.on_button_click([&bot](const dpp::button_click_t& event) {
        //dpp::guild_member::
        if(event.custom_id == "stop" && (event.command.usr.id.operator==(481963313433935888)|| event.command.usr.id.operator==(356099261831839744) || event.command.usr.id.operator==(296302730786373633)))
        {
            bot.message_delete(event.command.message_id, event.command.channel_id);
            system(SHUTDOWN_COMMAND);
        }
        if (event.custom_id == "del")
        {
            bot.message_delete(event.command.message_id, event.command.channel_id);
        }
    });

    /* Start the bot */
    bot.start(false);
    return 0;
}
