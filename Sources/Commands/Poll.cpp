#include "Commands/Poll.hpp"
#include "advancedFunction.hpp"

Commands::Poll::Poll(dpp::cluster& bot, Data& data) : ICommand(bot, data)
{
    name = "poll";
}

void Commands::Poll::Init(bool registerCommand)
{
    if (/*registerCommand &&*/ dpp::run_once<struct register_bot_commands>()) {
        dpp::slashcommand newcommand("poll", "Schedule a poll.", cp_bot.me.id);
        newcommand.set_default_permissions(dpp::p_administrator).
            add_option(
                dpp::command_option(dpp::co_sub_command, "add", "register a new Poll to the current chanel").
                add_option(dpp::command_option(dpp::co_string, "question", "The poll question.", true)).
                add_option(dpp::command_option(dpp::co_number, "expiry", "The life of the poll in hours.", true).set_min_value(1.0).set_max_value(336.0)).
                
                add_option(dpp::command_option(dpp::co_string, "answer_1", "The answer 1.", true)).
                add_option(dpp::command_option(dpp::co_string, "answer_2", "The answer 2.", true)).

                add_option(dpp::command_option(dpp::co_string, "date", "The date the poll will be sent. use the format YYYY-MM-DD HH:MM.", false)).
                add_option(dpp::command_option(dpp::co_boolean, "should_repeat", "On true send the poll every week. need to define a date first.", false)).

                add_option(dpp::command_option(dpp::co_string, "answer_1_emoji", "An emoji for the answer 1.",false)).
                add_option(dpp::command_option(dpp::co_string, "answer_2_emoji", "An emoji for the answer 2.", false))
        ).add_option(
            dpp::command_option(dpp::co_sub_command, "list", "List existing polls of the current chanel")
        ).add_option(
            dpp::command_option(dpp::co_sub_command, "delete", "Delete a poll with an id").add_option(dpp::command_option(dpp::co_integer, "id", "Poll id", true))
        );
        cp_bot.global_command_create(newcommand);
    }
}

void Commands::Poll::Execute(const dpp::slashcommand_t& event)
{
    if (event.command.get_command_name() == "poll") {
        const std::string subCommand = event.command.get_command_interaction().options[0].name;
        
        if (subCommand == "add")
        {
            PollData newPoll(std::get<std::string>(event.get_parameter("question")), std::get<double>(event.get_parameter("expiry")));

            std::string answer1 = std::get<std::string>(event.get_parameter("answer_1"));
            //answer 1
            /*if (std::holds_alternative<std::string>(event.get_parameter("answer_1_emoji")))
            {
                std::string e = std::get<std::string>(event.get_parameter("answer_1_emoji"));
                dpp::emoji emoji(e);
                poll.add_answer(answer1, emoji);
            }
            else
                poll.add_answer(answer1);*/

            std::string answer2 = std::get<std::string>(event.get_parameter("answer_2"));
   
            if (std::holds_alternative<std::string>(event.get_parameter("date")))
            {
                std::string datestr = std::get<std::string>(event.get_parameter("date"));
                Date dueDate;
                try
                {
                    dueDate = ParseDateTime(datestr);

                }
                catch (std::runtime_error error)
                {
                    event.reply("> Error invalid date format please retry");
                    return;
                }
                newPoll.SetDueDate(dueDate);
            }
            dpp::poll poll;
            poll.question.text = newPoll.title;
            poll.expiry = newPoll.duration;


            poll.add_answer(answer1,"");

            
            poll.add_answer(answer2);


            cp_bot.message_create(dpp::message(event.command.channel_id, "").set_poll(poll));

            //reply related
            std::string expirystr = std::to_string(newPoll.duration);
            expirystr.erase(expirystr.find_last_not_of('0') + 1, std::string::npos);
            std::string message = ">>> **:white_check_mark: Poll Created with:**\n- **Question:** " + newPoll.title + "\n- **Expiry in:** " + expirystr + "**h**";
            event.reply(dpp::message(message).set_flags(dpp::m_ephemeral));
            return;
        }else if (subCommand == "list")
        {
            event.reply(dpp::message("list").set_flags(dpp::m_ephemeral));
            return;
        }else if (subCommand == "delete")
        {
            event.reply(dpp::message("delete").set_flags(dpp::m_ephemeral));
            return;
        }


        Date today = advanced::GetActualDate();

        std::string message = "[ " + std::to_string(today.day) + " / " + std::to_string(today.month) + " / " + std::to_string(today.year) + "   |   " + std::to_string(today.hour) + " : " + std::to_string(today.minute) + " ]\nday of the week: " + std::to_string(today.dayOfWeek);

        /*dpp::poll poll;
        poll.question.text = "test";
        poll.expiry = 336.0;

        poll.add_answer("test2", "🍔");
        poll.add_answer("test2", "🍗");
        poll.add_answer("test1", "🍕");
        poll.add_answer("autre", "🤔");
        poll.add_answer("nah", "🚫");*/

        event.reply(dpp::message(message).set_flags(dpp::m_ephemeral));
        
    }
}

Date Commands::Poll::ParseDateTime(const std::string& _input)
{
    std::tm temp = {};
    std::istringstream ss(_input);
    ss >> std::get_time(&temp, "%Y-%m-%d %H:%M");

    if (ss.fail()) {
        throw std::runtime_error("Failed to parse datetime string");
    }

    Date result;
    result.day = temp.tm_mday;
    result.month = temp.tm_mon;
    result.year = temp.tm_year;

    result.hour = temp.tm_hour;
    result.minute = temp.tm_min;

    return result;
}