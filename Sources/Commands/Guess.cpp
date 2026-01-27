#include "Commands/Guess.hpp"

//- list of emoji id --------------
std::vector<uint64_t> Commands::Guess::blackID =
{
    1463959597420384501,
    1463959610967982232,
    1463959625723416586,
    1463959638964699279,
    1463959645046575246,
    1463959653816733767,
    1463959658166227007,
    1463959664059482227,
    1463959671063842847,
    1463959676436615168,
    1463959689535553790,
    1463959702781169757,
    1463959711027171587,
    1463959714797715680,
    1463959720254509096,
    1463959724675563641,
    1463959736146989107,
    1463959739896565790,
    1463959745512607826,
    1463959750738837702,
    1463959755549573183,
    1463959761002303643,
    1463959765305655340,
    1463959770246676716,
    1463959775405408412,
    1463959779226554419
};

std::vector<uint64_t> Commands::Guess::greenID =
{
    1463959600628760871,
    1463959614058922148,
    1463959630517502106,
    1463959640470720513,
    1463959646627954880,
    1463959655536656416,
    1463959660804440127,
    1463959666307498134,
    1463959672267735171,
    1463959681268449541,
    1463959694212075704,
    1463959704295178477,
    1463959712755089438,
    1463959716488155147,
    1463959721818980627,
    1463959733047267379,
    1463959737291898941,
    1463959740945141843,
    1463959747383525460,
    1463959751955185785,
    1463959757437010102,
    1463959762646335749,
    1463959767105146910,
    1463959772222193909,
    1463959776340738302,
    1463959780119941131
};

std::vector<uint64_t> Commands::Guess::yellowID =
{
    1463959606110978048,
    1463959623798358102,
    1463959631725334633,
    1463959641795854451,
    1463959651459665941,
    1463959657545596988,
    1463959662893465756,
    1463959667683364936,
    1463959674847105075,
    1463959683030319289,
    1463959697848668438,
    1463959706329419816,
    1463959713845874840,
    1463959717796778206,
    1463959722880139441,
    1463959734443966717,
    1463959738625560832,
    1463959742840963334,
    1463959749312647211,
    1463959753503019100,
    1463959759328776278,
    1463959764026261536,
    1463959768518361180,
    1463959773903982774,
    1463959778098417849,
    1463959781894258839
};
//---------------------------------

Commands::Guess::Guess(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data)
{
    wordToGuess = data.GetRandomWord();
    wrongChars.resize(24, false);
}

void Commands::Guess::Init(bool registerCommand, uint64_t _commandId)
{
    ICommand::Init(registerCommand, _commandId);
    if (dpp::run_once<struct register_bot_commands>())
    {
        command = dpp::slashcommand(name, "Guess the word.", cp_bot.me.id);
        command.add_option(dpp::command_option(dpp::co_string, "word", "A 5 letter word", true));

        if (registerCommand) {
            cp_bot.global_command_create(command);
        }
    }
}

void Commands::Guess::Execute(const dpp::slashcommand_t& event)
{
    ICommand::Execute(event);
    if (event.command.get_command_name() == name) {
        std::string userWord = std::get<std::string>(event.get_parameter("word"));
        if (userWord.length() != 5)
        {
            event.reply(dpp::message("Must be a 5 letter word.").set_flags(dpp::m_ephemeral));
            return;
        }
        std::transform(userWord.begin(), userWord.end(), userWord.begin(),
            [](unsigned char c) { return std::tolower(c); });

        if (!cp_data.IsValid(userWord))
        {
            event.reply(dpp::message("Must be a real word.").set_flags(dpp::m_ephemeral));
            return;
        }
        dpp::message temp;
        for (int i = 0; i < 5; i++)
        {
            char userC = userWord[i];
            char guessC = wordToGuess[i];
            if (userC == guessC) temp.content += std::string("<:") + userC + ":" + std::to_string(greenID[userC - 97]) + ">";
            else if ([&]() { for (int j = i + 1; j < 5; j++) { if(userC == wordToGuess[j]) return true;}return false; }()) temp.content += std::string("<:") + userC + ":" + std::to_string(yellowID[userC - 97]) + ">";
            else
            {
                temp.content += std::string("<:") + userC + ":" + std::to_string(blackID[userC - 97]) + ">";
                wrongChars[userC - 97] = true;
                asWrongChar = true;
            }
        }
        if (asWrongChar)
        {
            temp.content += "\n-# ";
            for (int i = 0; i < 24; i++)
            {
                if (!wrongChars[i]) continue;
                temp.content += std::string("<:") + char(i+97) + ":" + std::to_string(blackID[i]) + ">";
            } 
        }
        if(userWord == wordToGuess)
            wordToGuess = cp_data.GetRandomWord();
        /*int index = lastRandom;
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
        };*/


        event.reply(temp);
    }
}