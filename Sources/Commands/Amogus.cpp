#include "Commands/Amogus.hpp"

Commands::Amogus::Amogus(dpp::cluster& bot, Data& data) : ICommand(bot, data)
{
    rng.seed(time(NULL));
    //AMOGUSGenerator = std::uniform_int_distribution<int>(0, data.GetSusImages().size() - 1);
}

void Commands::Amogus::Init()
{
    if (dpp::run_once<struct register_bot_commands>()) {
        cp_bot.global_command_create(dpp::slashcommand("amogus", "did you say AMOGUS?", cp_bot.me.id));
    }
}

void Commands::Amogus::Execute(const dpp::slashcommand_t& event)
{
    if (event.command.get_command_name() == "amogus") {
        dpp::message temp;

        int index;
        if (cp_data.GetSusImages().size() > 1)
        {
            index = AMOGUSGenerator(rng);
            while (index == lastRandom) index = AMOGUSGenerator(rng);
            lastRandom = index;
        }
        else index = 0;
        const auto& result = cp_data.GetSusImages()[index];
        temp.add_file((std::string("AMOGUS").append(result.extension().string())), dpp::utility::read_file(result.string()));

        event.reply(temp);
    }
}

void Commands::Amogus::Reload()
{
    AMOGUSGenerator = std::uniform_int_distribution<int>(0, cp_data.GetSusImages().size() - 1);
}