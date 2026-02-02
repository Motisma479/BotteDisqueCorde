#include "Commands/Amogus.hpp"

Commands::Amogus::Amogus(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data)
{
    rng.seed(time(NULL));
    AMOGUSGenerator = std::uniform_int_distribution<int>(0, data.GetSusImages().size() - 1);
}

void Commands::Amogus::Init(CommandIds _commandIds)
{
    if (dpp::run_once<struct register_bot_commands>())
    {
        commands.chatCommand = new dpp::slashcommand(name, "did you say AMOGUS?", cp_bot.me.id);

        ICommand::Init(_commandIds);
    }
}

void Commands::Amogus::Execute(const dpp::slashcommand_t& event)
{
    ICommand::Execute(event);
    if (event.command.get_command_name() == name) {
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

void Commands::Amogus::Reload(bool reRegister)
{
    ICommand::Reload(reRegister);
    AMOGUSGenerator = std::uniform_int_distribution<int>(0, cp_data.GetSusImages().size() - 1);
}