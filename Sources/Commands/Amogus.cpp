#include "Commands/Amogus.hpp"

Commands::Amogus::Amogus(dpp::cluster& bot, Data& data) : ICommand(bot, data)
{
    name = "amogus";

    rng.seed(time(NULL));
    AMOGUSGenerator = std::uniform_int_distribution<int>(0, data.GetSusImages().size() - 1);
}

void Commands::Amogus::Init(bool registerCommand)
{
    if (registerCommand && dpp::run_once<struct register_bot_commands>()) {
        cp_bot.global_command_create(dpp::slashcommand("amogus", "did you say AMOGUS?", cp_bot.me.id));
    }
}

void Commands::Amogus::Execute(const dpp::slashcommand_t& event)
{
    if (event.command.get_command_name() == "amogus") {
        dpp::message temp;

        int index = lastRandom;
        if (cp_data.GetSusImages().size() > 1)
        {
            while (index == lastRandom) index = AMOGUSGenerator(rng);
            lastRandom = index;

            const auto& result = cp_data.GetSusImages()[index];
            temp.add_file((std::string("AMOGUS").append(result.extension().string())), dpp::utility::read_file(result.string()));
        }
        else
        {
            temp.content = "No file founded";
            temp.set_flags(dpp::m_ephemeral);
        };
        

        event.reply(temp);
    }
}

void Commands::Amogus::Reload()
{
    AMOGUSGenerator = std::uniform_int_distribution<int>(0, cp_data.GetSusImages().size() - 1);
}