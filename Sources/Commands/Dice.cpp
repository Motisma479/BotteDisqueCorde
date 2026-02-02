#include "Commands/Dice.hpp"

Commands::Dice::Dice(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data)
{
    rng.seed(time(NULL));
}

void Commands::Dice::Init(CommandIds _commandIds)
{
    if (dpp::run_once<struct register_bot_commands>())
    {
        commands.chatCommand = new dpp::slashcommand(name, "roll a dice", cp_bot.me.id);
        commands.chatCommand->add_option(
            dpp::command_option(dpp::co_integer, "face", "the number face", false)
        ).set_dm_permission(true);

        ICommand::Init(_commandIds);
    }
}

void Commands::Dice::Execute(const dpp::slashcommand_t& event)
{
    ICommand::Execute(event);
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
        std::string temp = ":game_die:: **" + std::to_string(diceGenerator(rng)) + "**";
        event.reply(temp);
    }
}