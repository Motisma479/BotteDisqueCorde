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
            dpp::command_option(dpp::co_sub_command,"add","register a new Poll to the current chanel").
                add_option(dpp::command_option(dpp::co_string,"question","The poll question.",true)).
                add_option(dpp::command_option(dpp::co_number, "expiry", "The life of the poll in hours.", true).set_min_value(1.0).set_max_value(336.0))
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
            std::string pollQuestion = std::get<std::string>(event.get_parameter("question"));
            double pollExpiry = std::get<double>(event.get_parameter("expiry"));


            dpp::poll poll;
            poll.question.text = pollQuestion;
            poll.expiry = pollExpiry;
            poll.add_answer("test2", "🍔");
            poll.add_answer("test2", "🍗");

            cp_bot.message_create(dpp::message(event.command.channel_id, "").set_poll(poll));

            //reply related
            std::string expirystr = std::to_string(pollExpiry);
            expirystr.erase(expirystr.find_last_not_of('0') + 1, std::string::npos);
            std::string message = ">>> **:white_check_mark: Poll Created with:**\n- **Question:** " + pollQuestion + "\n- **Expiry in:** " + expirystr + "**h**";
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


        date today = advanced::GetActualDate();

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