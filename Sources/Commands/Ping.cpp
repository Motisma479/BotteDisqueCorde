#include "Commands/Ping.hpp"

Commands::Ping::Ping(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data) {}

void Commands::Ping::Init(bool registerCommand, uint64_t _commandId)
{
    ICommand::Init(registerCommand, _commandId);
    if (dpp::run_once<struct register_bot_commands>())
    {
        command = dpp::slashcommand(name, "Ping pong!", cp_bot.me.id);

        if (registerCommand) {
            cp_bot.global_command_create(command);
        }
    }
}

void Commands::Ping::Execute(const dpp::slashcommand_t& event)
{
    ICommand::Execute(event);
    if (event.command.get_command_name() == name) {
        double latency_ms = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch() ).count() - static_cast<long long>(event.command.get_creation_time() * 1000.0);
        auto api_start = std::chrono::steady_clock::now();
       
        std::string temp = "**:ping_pong:Pong!**\n▬▬▬▬▬▬▬▬▬▬\n> **latency:**\n> " + std::to_string(latency_ms) + " **ms**" + "\n▬▬▬▬▬▬▬▬▬▬\n> **API:**\n> <a:loading:1362431721399783505>";
        event.reply(temp, [event, latency_ms, api_start](const dpp::confirmation_callback_t& callback) {
            auto api_end = std::chrono::steady_clock::now();
            double api_ms = std::chrono::duration_cast<std::chrono::milliseconds>(api_end - api_start).count();
            
            std::string after = "**:ping_pong:Pong!**\n▬▬▬▬▬▬▬▬▬▬\n> **latency:**\n> " + std::to_string(latency_ms) + " **ms**" + "\n▬▬▬▬▬▬▬▬▬▬\n> **API:**\n> " + std::to_string(api_ms) + " **ms**";;
            event.edit_original_response(dpp::message(after));
        });
    }
}