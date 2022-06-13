// BotteDisqueCorde.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
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

#define MAX_SUS_IMAGES 10

int main()
{
    srand(time(NULL));

    static int lastRandom = -1;
    /* Create bot cluster */
    dpp::cluster bot(BOT_TOKEN);

    /* Output simple log messages to stdout */
    bot.on_log(dpp::utility::cout_logger());

    /* Handle slash command */
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }
        if (event.command.get_command_name() == "amogus") {
            dpp::message temp;

            int index;
            if (MAX_SUS_IMAGES > 1)
            {
                index = rand() % MAX_SUS_IMAGES;
                while (index == lastRandom) index = rand() % MAX_SUS_IMAGES;
            }
            else index = 0;
            char ImagePath[25] = "Resources/AMOGUS/XXX.png";
            ImagePath[17] = ((index / 100) % 10) + '0';
            ImagePath[18] = ((index / 10) % 10) + '0';
            ImagePath[19] = (index % 10) + '0';
            temp.add_file("AMOGUS.png", dpp::utility::read_file(ImagePath));

            event.reply(temp);
        }
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
        });


    /* Start the bot */
    bot.start(false);
    return 0;
}
