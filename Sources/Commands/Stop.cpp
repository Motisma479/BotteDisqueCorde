#include "Commands/Stop.hpp"

void Commands::Stop::Init()
{
    if (dpp::run_once<struct register_bot_commands>()) {
        dpp::slashcommand newcommand("stop", "stop the bot.", cp_bot.me.id);
        newcommand.set_default_permissions(dpp::p_administrator);
        cp_bot.global_command_create(newcommand);
    }
}

void Commands::Stop::Execute(const dpp::slashcommand_t& event)
{
    if (event.command.get_command_name() == "stop") {
        dpp::message temp;
        temp.add_component(
            dpp::component()
            .add_component(
                dpp::component().set_emoji("✔").
                set_type(dpp::cot_button).
                set_style(dpp::cos_secondary).
                set_id("stop")
            ).add_component(
                dpp::component().set_emoji("❌").
                set_type(dpp::cot_button).
                set_style(dpp::cos_secondary).
                set_id("del")
            )
        );
        event.reply(temp);
    }
}