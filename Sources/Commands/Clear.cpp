#include "Commands/Clear.hpp"

dpp::cluster* _bot;

Commands::Clear::Clear(dpp::cluster& bot, Data& data) : ICommand(bot, data)
{
    _bot = &bot;
}

void callback(const dpp::confirmation_callback_t& event)
{
    if (event.is_error())
        return;
    std::vector<dpp::snowflake> msgids;
    dpp::message_map object = std::get<dpp::message_map>(event.value);
    dpp::snowflake chanelId = object.begin()->second.channel_id;
    for (auto m = object.begin(); m != object.end(); m++)
        //bot.message_delete(m->second.id, m->second.channel_id);
        msgids.push_back(m->second.id);
    if (msgids.size() == 1)//if delete only 1
        _bot->message_delete(msgids[0], chanelId);
    else
       _bot->message_delete_bulk(msgids, chanelId);
    return;
}

void Commands::Clear::Init()
{
    if (dpp::run_once<struct register_bot_commands>()) {
        dpp::slashcommand newcommand("clear", "clear the chanel", cp_bot.me.id);
        newcommand.add_option(
            dpp::command_option(dpp::co_integer, "count", "the number of message to delete", false)
        );
        cp_bot.global_command_create(newcommand);
    }
}

void Commands::Clear::Execute(const dpp::slashcommand_t& event)
{
    if (event.command.get_command_name() == "clear") {
        int value;
        if (event.get_parameter("count").index() <= 0)
            value = 1;
        else
        {
            value = std::get<int64_t>(event.get_parameter("count"));
            if (value <= 0)
                value = 1;
        }


        cp_bot.messages_get(event.command.channel_id, 0, event.command.message_id, 0, value, callback);
        //cp_bot.mess
        event.reply(dpp::message("complete").set_flags(dpp::m_ephemeral));
    }
}