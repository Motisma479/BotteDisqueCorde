#include "Commands/Dice.hpp"

Commands::Dice::Dice(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data)
{
    rng.seed(time(NULL));
}

void Commands::Dice::Init(bool registerCommand, uint64_t _commandId)
{
    if (registerCommand && dpp::run_once<struct register_bot_commands>()) {
        dpp::slashcommand newcommand(name, "roll a dice", cp_bot.me.id);
        newcommand.add_option(
            dpp::command_option(dpp::co_integer, "face", "the number face", false)
        );
        cp_bot.global_command_create(newcommand);
    }
}

void Commands::Dice::Execute(const dpp::slashcommand_t& event)
{
    if (event.command.get_command_name() == name) {
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
}