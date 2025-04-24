#include "Commands/Poll.hpp"
#include "advancedFunction.hpp"

#include <regex>
#include <codecvt>


Commands::Poll::Poll(dpp::cluster& bot, Data& data, PollManager& manager) : ICommand(bot, data), pollManager(manager)
{
    name = "poll";
}

void Commands::Poll::Init(bool registerCommand)
{
    if (registerCommand && dpp::run_once<struct register_bot_commands>()) {
        dpp::slashcommand newcommand("poll", "Schedule a poll.", cp_bot.me.id);
        newcommand.set_default_permissions(dpp::p_administrator).
            add_option(
                dpp::command_option(dpp::co_sub_command, "add", "register a new Poll to the current chanel").
                add_option(dpp::command_option(dpp::co_string, "question", "The poll question.", true).set_max_length(300)).
                add_option(dpp::command_option(dpp::co_number, "expiry", "The life of the poll in hours.", true).set_min_value(1.0).set_max_value(336.0)).
                
                add_option(dpp::command_option(dpp::co_string, "answer_1", "The answer 1.", true)).
                add_option(dpp::command_option(dpp::co_string, "answer_2", "The answer 2.", true)).

                add_option(dpp::command_option(dpp::co_string, "date", "The date the poll will be sent. use the format YYYY-MM-DD HH:MM.", false)).
                add_option(dpp::command_option(dpp::co_boolean, "should_repeat", "On true send the poll every week. need to define a date first.", false)).

                add_option(dpp::command_option(dpp::co_string, "answer_3", "The answer 3.", false).set_max_length(55)).
                add_option(dpp::command_option(dpp::co_string, "answer_4", "The answer 4.", false).set_max_length(55)).
                add_option(dpp::command_option(dpp::co_string, "answer_5", "The answer 5.", false).set_max_length(55)).
                add_option(dpp::command_option(dpp::co_string, "answer_6", "The answer 6.", false).set_max_length(55)).
                add_option(dpp::command_option(dpp::co_string, "answer_7", "The answer 7.", false).set_max_length(55)).
                add_option(dpp::command_option(dpp::co_string, "answer_8", "The answer 8.", false).set_max_length(55)).
                add_option(dpp::command_option(dpp::co_string, "answer_9", "The answer 9.", false).set_max_length(55)).
                add_option(dpp::command_option(dpp::co_string, "answer_10", "The answer 10.", false).set_max_length(55)).

                add_option(dpp::command_option(dpp::co_string, "answer_1_emoji", "An emoji for the answer 1.",false)).
                add_option(dpp::command_option(dpp::co_string, "answer_2_emoji", "An emoji for the answer 2.", false)).
                add_option(dpp::command_option(dpp::co_string, "answer_3_emoji", "An emoji for the answer 3.", false)).
                add_option(dpp::command_option(dpp::co_string, "answer_4_emoji", "An emoji for the answer 4.", false)).
                add_option(dpp::command_option(dpp::co_string, "answer_5_emoji", "An emoji for the answer 5.", false)).
                add_option(dpp::command_option(dpp::co_string, "answer_6_emoji", "An emoji for the answer 6.", false)).
                add_option(dpp::command_option(dpp::co_string, "answer_7_emoji", "An emoji for the answer 7.", false)).
                add_option(dpp::command_option(dpp::co_string, "answer_8_emoji", "An emoji for the answer 8.", false)).
                add_option(dpp::command_option(dpp::co_string, "answer_9_emoji", "An emoji for the answer 9.", false)).
                add_option(dpp::command_option(dpp::co_string, "answer_10_emoji", "An emoji for the answer 10.", false))

        ).add_option(
            dpp::command_option(dpp::co_sub_command, "list", "List existing polls of the current chanel")
        ).add_option(
            dpp::command_option(dpp::co_sub_command, "delete", "Delete a poll with an id").add_option(dpp::command_option(dpp::co_integer, "id", "Poll id", true).set_min_value(0).set_max_value(65535))
        );
        cp_bot.global_command_create(newcommand);
    }
}


static std::u32string utf8_to_u32(const std::string& s) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.from_bytes(s);
}

static bool is_emoji_codepoint(char32_t cp) {
    // These ranges cover most “vanilla” emojis; add more as needed
    static const std::vector<std::pair<char32_t, char32_t>> R = {
        {0x1F300, 0x1F5FF}, // Misc symbols & pictographs
        {0x1F600, 0x1F64F}, // Emoticons
        {0x1F680, 0x1F6FF}, // Transport & map
        {0x1F700, 0x1F77F}, // Alchemical symbols
        {0x1F780, 0x1F7FF}, // Geometric shapes extended
        {0x1F800, 0x1F8FF}, // Supplemental arrows
        {0x1F900, 0x1F9FF}, // Supplemental symbols & pictographs
        {0x1FA00, 0x1FA6F}, // Chess symbols, symbols & pictographs
        {0x2600,  0x26FF }, // Misc symbols (e.g. ☀️–⛅)
        {0x2700,  0x27BF }, // Dingbats
    };
    for (auto& range : R) {
        if (cp >= range.first && cp <= range.second) return true;
    }
    return false;
}

bool IsAnEmoji(const std::string& input) {
    // Regular expression pattern to match common emoji ranges

    std::regex emoji_regex(R"(<a?:([a-zA-Z0-9_]+):(\d+)>)");
    std::smatch match;
    if (std::regex_match(input, match, emoji_regex))
    {
        return true;
    }
    else
    {
        auto u32 = utf8_to_u32(input);
        return (u32.size() == 1) && is_emoji_codepoint(u32[0]);
    }
}

void Commands::Poll::Execute(const dpp::slashcommand_t& event)
{
    if (event.command.get_command_name() == "poll") {
        const std::string subCommand = event.command.get_command_interaction().options[0].name;
        
        if (subCommand == "add")
        {
            PollData newPoll(std::get<std::string>(event.get_parameter("question")), std::get<double>(event.get_parameter("expiry")));

            for (int i = 1; i < 10; i++)
            {
                char answer[9] = "answer_x";
                answer[7] = 48 + i;
                char answerEmoji[15] = "answer_x_emoji";
                answerEmoji[7] = 48 + i;

                if (std::holds_alternative<std::string>(event.get_parameter(answer)))
                {
                    if (std::holds_alternative<std::string>(event.get_parameter(answerEmoji)))
                    {
                        std::string emo = std::get<std::string>(event.get_parameter(answerEmoji));
                        if (!IsAnEmoji(emo))
                        {
                            event.reply(dpp::message("> **Error: invalid emoji for the answer " + std::to_string(i) + " please retry.**").set_flags(dpp::m_ephemeral));
                            return;
                        }
                        newPoll.answers.push_back({ std::get<std::string>(event.get_parameter(answer)), emo});
                    }
                    else
                        newPoll.answers.push_back({ std::get<std::string>(event.get_parameter(answer)),"" });
                }
            }
   
            if (std::holds_alternative<std::string>(event.get_parameter("date"))) //if the poll as a due date register it to the manager
            {
                std::string datestr = std::get<std::string>(event.get_parameter("date"));
                Date dueDate;
                try
                {
                    dueDate = advanced::ParseDateTime(datestr);
                }
                catch (std::runtime_error error)
                {
                    event.reply(dpp::message("> **Error: invalid date format please retry.**").set_flags(dpp::m_ephemeral));
                    return;
                }
                newPoll.SetDueDate(dueDate);
                if (std::holds_alternative<bool>(event.get_parameter("should_repeat"))) //check if it should repeat
                {
                    newPoll.shouldRepeat = std::get<bool>(event.get_parameter("should_repeat"));
                }

                pollManager.Add(newPoll, event.command.channel_id);
                
                //reply
                std::string expirystr = std::to_string(newPoll.duration);
                expirystr.erase(expirystr.find_last_not_of('0') + 1, std::string::npos);
                std::string message = ">>> **:white_check_mark: Poll Created with:**\n- **Question:** " + newPoll.title +
                    "\n- **Expiry in:** " + expirystr + "**h**" +
                    "\n- **Due date:** " + datestr +
                    "\n- **Will repeat:** " + (newPoll.shouldRepeat?"true":"false") + "\n- **answer: **\n";
                for (const auto [answer, emoji] : newPoll.answers)
                {
                    if(emoji.size() > 0)
                        message += "   - " + emoji + ", " + answer + '\n';
                    else
                        message += "   - " + answer + '\n';
                }
                event.reply(dpp::message(message).set_flags(dpp::m_ephemeral));
            }
            else
            {
                dpp::poll poll;
                poll.question.text = newPoll.title;
                poll.expiry = newPoll.duration;
                for (const auto [answer, emoji] : newPoll.answers)
                {
                    //dpp::emoji temp(":image1:");
                    dpp::emoji temp;

                    std::regex emoji_regex(R"(<a?:([a-zA-Z0-9_]+):(\d+)>)");
                    std::smatch match;
                    if (std::regex_match(emoji, match, emoji_regex)) {
                        temp.name = match[1];
                        temp.id = dpp::snowflake(std::string(match[2]));
                        if(emoji[1] == 'a')
                            temp.flags |= dpp::e_animated;
                    }
                    else
                    {
                        temp.name = emoji;
                    }

                    poll.add_answer(answer, temp);
                }
                cp_bot.message_create(dpp::message(event.command.channel_id, "").set_poll(poll));
                event.reply(dpp::message("> **:white_check_mark: Poll Created.**").set_flags(dpp::m_ephemeral));
            }
        
        }else if (subCommand == "list")
        {
            Date today = advanced::GetActualDate();
            std::string message = ">>> **Current time: [ " + std::to_string(today.day) + " / " + std::to_string(today.month) + " / " + std::to_string(today.year) + "   |   " + std::to_string(today.hour) + " : " + std::to_string(today.minute) + " ]   day of the week: " + std::to_string(today.dayOfWeek) + "**\n\n";
            
            for (const auto& data : pollManager.List(event.command.channel_id))
            {
                message += '`' + data.title + "`\n";
                Date d = data.GetDueDate();
                message += "- **ID: " + std::to_string(data.id) + (data.shouldRepeat ? "   Day of week: " +
                    std::to_string(d.dayOfWeek) : "   Date: " + std::to_string(d.year) + '-' + (d.month < 10 ? ("0" + std::to_string(d.month)) : std::to_string(d.month)) + '-' + (d.day < 10 ? ("0" + std::to_string(d.day)) : std::to_string(d.day)) + ' ' + (d.hour < 10 ? ("0" + std::to_string(d.hour)) : std::to_string(d.hour)) + ':' + (d.minute < 10 ? ("0" + std::to_string(d.minute)) : std::to_string(d.minute))) + "**\n\n";
            }

            event.reply(dpp::message(message).set_flags(dpp::m_ephemeral));

        }else if (subCommand == "delete")
        {
            uint16_t id = static_cast<uint16_t>(std::get<int64_t>(event.get_parameter("id")));//should be "safe" since the user cant send overflowing value;
            switch (pollManager.Delete(id, event.command.channel_id))
            {
            case 1:
                event.reply(dpp::message("> **Error: poll access denied.**").set_flags(dpp::m_ephemeral));
                break;
            case 2:
                event.reply(dpp::message("> **Error: no poll found.**").set_flags(dpp::m_ephemeral));
                break;
            default://0
                event.reply(dpp::message("> **:white_check_mark: Poll** __" + std::to_string(id) + "__ **deleted.**").set_flags(dpp::m_ephemeral));
                break;
            }
        }        
    }
}