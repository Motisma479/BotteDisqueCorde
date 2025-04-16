#include "Commands/Meme.hpp"

Commands::Meme::Meme(dpp::cluster& bot, Data& data) : ICommand(bot, data)
{
    name = "meme";
    rng.seed(time(NULL));
    memeGenerator = std::uniform_int_distribution<int>(0, data.GetMemeImages().size() - 1);
}

void Commands::Meme::Init(bool registerCommand)
{
    if (registerCommand && dpp::run_once<struct register_bot_commands>()) {
        cp_bot.global_command_create(dpp::slashcommand("meme", "Send a meme.", cp_bot.me.id));
    }
}

void Commands::Meme::Execute(const dpp::slashcommand_t& event)
{
    if (event.command.get_command_name() == "meme") {
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

void Commands::Meme::Reload()
{
    memeGenerator = std::uniform_int_distribution<int>(0, cp_data.GetMemeImages().size() - 1);
}