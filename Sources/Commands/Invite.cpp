#include "Commands/Invite.hpp"

Commands::Invite::Invite(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data) {}

void Commands::Invite::Init(bool registerCommand, uint64_t _commandId)
{
    ICommand::Init(registerCommand, _commandId);
    if (dpp::run_once<struct register_bot_commands>())
    {
        command = dpp::slashcommand(name, "invite a user to play some game", cp_bot.me.id);
        command.add_option(
            dpp::command_option(dpp::co_string, "game", "The game to play", true).
            add_choice(dpp::command_option_choice("chess", std::string("game_chess")))
        ).add_option(
            dpp::command_option(dpp::co_user, "user", "User to play with", true)
        );
    
        if (registerCommand)
        {
            cp_bot.global_command_create(command);
        }
    }
}

void Commands::Invite::Execute(const dpp::slashcommand_t& event)
{
    ICommand::Execute(event);
    if (event.command.get_command_name() == name) {

        std::string game = std::get<std::string>(event.get_parameter("game"));
        dpp::snowflake invitated_id = std::get<dpp::snowflake>(event.get_parameter("user"));
        dpp::user invitated = event.command.get_resolved_user(invitated_id);

        dpp::embed embed = dpp::embed().
            set_color(0xAA8ED6).
            set_author(invitated.username, "", invitated.get_avatar_url()).
            set_description(invitated.get_mention() + "\n———VS———\n" + event.command.member.get_mention()).
            set_thumbnail("https://dpp.dev/DPP-Logo.png").
            add_field(
                "Do you accept ?",
                "———————"
            ).
            set_footer(dpp::embed_footer().set_text("Invited by " + event.command.usr.username).set_icon(event.command.usr.get_avatar_url())).
            set_timestamp(time(0));
        event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
        dpp::message temp(event.command.channel_id, "");
        temp.add_component(
            dpp::component()
            .add_component(
                dpp::component().set_emoji("✔").
                set_type(dpp::cot_button).
                set_style(dpp::cos_secondary).
                set_id("dal")
            ).add_component(
                dpp::component().set_emoji("❌").
                set_type(dpp::cot_button).
                set_style(dpp::cos_secondary).
                set_id("del")
            )
        );
        cp_bot.message_create(temp);
    }
}

/*
     dpp::embed embed = dpp::embed().
                set_color(0xAA8ED6).
                set_author("Some name", "https://dpp.dev/", "https://dpp.dev/DPP-Logo.png").
                set_description("Some description here").
                set_thumbnail("https://dpp.dev/DPP-Logo.png").
                add_field(
                    "Regular field title",
                    "Some value here"
                ).
                add_field(
                    "Inline field title",
                    "Some value here",
                    true
                ).
                add_field(
                    "Inline field title",
                    "Some value here",
                    true
                ).
                set_image("https://dpp.dev/DPP-Logo.png").
                set_footer(dpp::embed_footer().set_text("Some footer text here").set_icon("https://dpp.dev/DPP-Logo.png")).
                set_timestamp(time(0));
                event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
    */