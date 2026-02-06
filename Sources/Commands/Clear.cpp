#include "Commands/Clear.hpp"

dpp::cluster* _bot;

Commands::Clear::Clear(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data)
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

void Commands::Clear::Init(CommandIds _commandIds)
{
    if (dpp::run_once<struct register_bot_commands>())
    {
        commands.chatCommand = new dpp::slashcommand(name, "clear the chanel", cp_bot.me.id);
        commands.chatCommand->add_option(
            dpp::command_option(dpp::co_integer, "count", "the number of message to delete", false)
        ).set_default_permissions(dpp::p_administrator);
        
        ICommand::Init(_commandIds);    
    }
}

void Commands::Clear::Execute(const dpp::slashcommand_t& event)
{
    ICommand::Execute(event);
    if (event.command.get_command_name() == name) {
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

        std::ostringstream response;
        response << ":white_check_mark: **complete**\n> :wastebasket: message deleted: **" << value<<"**";
        event.reply(dpp::message(response.str()).set_flags(dpp::m_ephemeral));
    }
}