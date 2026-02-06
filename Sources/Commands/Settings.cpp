#include "Commands/Settings.hpp"
#include "advancedFunction.hpp"

Commands::Settings::Settings(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data) {}

void Commands::Settings::Init(CommandIds _commandIds)
{
    if (dpp::run_once<struct register_bot_commands>())
    {
        commands.chatCommand = new dpp::slashcommand(name, "Allow a Super Admin to edit the bot settings.", cp_bot.me.id);
        commands.chatCommand->set_dm_permission(true).set_default_permissions(dpp::p_administrator);

        ICommand::Init(_commandIds);
    }
}

void Commands::Settings::Execute(const dpp::slashcommand_t& _event)
{
    ICommand::Execute(_event);
    if (_event.command.get_command_name() == name) {
        if (!advanced::CheckSuperAdminID(_event.command.get_issuing_user().id))
        {
            _event.reply(dpp::message("Only super admin can use that command.").set_flags(dpp::m_ephemeral));
            return;
        }

        dpp::interaction_modal_response modal("settings_modal", "Please adjust the settings");
        /* Add a text component */
        modal.add_component(
            dpp::component()
            .set_label("Stop machine")
            .set_type(dpp::cot_selectmenu)
            .set_id("shutdownValue")
            .add_select_option(
                dpp::select_option()
                .set_label("true")
                .set_value("true")
                .set_default(cp_data.GetStopMachine())
            )
            .add_select_option(
                dpp::select_option()
                .set_label("false")
                .set_value("false")
                .set_default(!cp_data.GetStopMachine())
            )
            .set_required(false)
        )
        .add_component(
            dpp::component()
            .set_label("Pressence message")
            .set_type(dpp::cot_text)
            .set_text_style(dpp::text_short)
            .set_id("pressenceMessage")
            .set_placeholder(cp_data.GetPressenceMessage())
            .set_default_value(cp_data.GetPressenceMessage())
        ).add_component(
            dpp::component()
            .set_label("Latitude")
            .set_type(dpp::cot_text)
            .set_text_style(dpp::text_short)
            .set_id("latitude")
            .set_placeholder(std::to_string(cp_data.GetLatitude()))
            .set_default_value(std::to_string(cp_data.GetLatitude()))
        ).add_component(
            dpp::component()
            .set_label("Longitude")
            .set_type(dpp::cot_text)
            .set_text_style(dpp::text_short)
            .set_id("longitude")
            .set_placeholder(std::to_string(cp_data.GetLongitude()))
            .set_default_value(std::to_string(cp_data.GetLongitude()))
        ).add_component(
            dpp::component()
            .set_label("Metrics")
            .set_type(dpp::cot_selectmenu)
            .set_id("metrics")
            .add_select_option(
                dpp::select_option()
                .set_label("standard")
                .set_value("standard")
                .set_default(cp_data.GetMetrics() == "standard")
            )
            .add_select_option(
                dpp::select_option()
                .set_label("metric")
                .set_value("metric")
                .set_default(cp_data.GetMetrics() == "metric")
            )
            .add_select_option(
                dpp::select_option()
                .set_label("imperial")
                .set_value("imperial")
                .set_default(cp_data.GetMetrics() == "imperial")
            )
            .set_required(false)
        );

        _event.dialog(modal);

    }
}