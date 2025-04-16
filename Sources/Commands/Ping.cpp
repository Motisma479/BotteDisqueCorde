#include "Commands/Ping.hpp"

Commands::Ping::Ping(dpp::cluster& bot, Data& data) : ICommand(bot, data)
{
    name = "ping";
}

void Commands::Ping::Init(bool registerCommand)
{
    if (registerCommand && dpp::run_once<struct register_bot_commands>()) {
        cp_bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", cp_bot.me.id));
    }
}

void Commands::Ping::Execute(const dpp::slashcommand_t& event)
{
    if (event.command.get_command_name() == "ping") {
        double APIH = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
        APIH /= 1000;
        double result = (APIH - event.command.get_creation_time()) * 1000;
        std::string temp = "🏓Pong!\n------------------------\nlatency:\n" + std::to_string(abs(result)) + " ms" + "\n------------------------\nAPI:\n" + std::to_string(APIH - std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count() / 1000) + " ms";
        event.reply(temp);

    }
}