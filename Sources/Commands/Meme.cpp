#include "Commands/Meme.hpp"

Commands::Meme::Meme(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data)
{
    rng.seed(time(NULL));
    memeGenerator = std::uniform_int_distribution<int>(0, data.GetMemeImages().size() - 1);
}

void Commands::Meme::Init(CommandIds _commandIds)
{
    if (dpp::run_once<struct register_bot_commands>())
    {
        commands.chatCommand = new dpp::slashcommand(name, "Send a meme.", cp_bot.me.id);

        ICommand::Init(_commandIds);
    }
}

void Commands::Meme::Execute(const dpp::slashcommand_t& event)
{
    ICommand::Execute(event);
    if (event.command.get_command_name() == name) {
        dpp::message temp;

        int index = lastRandom;
        if (cp_data.GetMemeImages().size() > 1)
        {
            while (index == lastRandom) index = memeGenerator(rng);
            lastRandom = index;
            const auto& result = cp_data.GetMemeImages()[index];
            temp.add_file((std::string("MEEEM").append(result.extension().string())), dpp::utility::read_file(result.string()));
        }
        else
        {
            temp.content = "No file founded";
            temp.set_flags(dpp::m_ephemeral);
        };

        event.reply(temp);
    }
}

void Commands::Meme::Reload(bool reRegister)
{
    ICommand::Reload(reRegister);
    memeGenerator = std::uniform_int_distribution<int>(0, cp_data.GetMemeImages().size() - 1);
}