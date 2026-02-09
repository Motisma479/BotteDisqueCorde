#include "Commands/Upload.hpp"
#include "advancedFunction.hpp"

Commands::Upload::Upload(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data) {}

void Commands::Upload::Init(CommandIds _commandIds)
{
    if (dpp::run_once<struct register_bot_commands>())
    {
        commands.chatCommand = new dpp::slashcommand(name, "Upload files.", cp_bot.me.id);
        commands.chatCommand->set_default_permissions(dpp::p_administrator);

        ICommand::Init(_commandIds);
    }
}

void Commands::Upload::Execute(const dpp::slashcommand_t& _event)
{
    ICommand::Execute(_event);
    if (_event.command.get_command_name() == name) {
        if (!advanced::CheckSuperAdminID(_event.command.get_issuing_user().id))
        {
            _event.reply(dpp::message("Only super admin can use that command.").set_flags(dpp::m_ephemeral));
            return;
        }

        _event.reply(dpp::message("**Comming soon!**\n-# it's already fully implemented but due to a bug in D++ it is postponed.").set_flags(dpp::m_ephemeral));
        return;

        dpp::interaction_modal_response modal("upload_modal", "Upload files:");
        /* Add a text component */
        modal.add_component(
            dpp::component()
            .set_label("Where to upload")
            .set_type(dpp::cot_selectmenu)
            .set_id("whereToUpload")
            .add_select_option(
                dpp::select_option()
                .set_label("amogus")
                .set_value("amogus")
            )
            .add_select_option(
                dpp::select_option()
                .set_label("meme")
                .set_value("meme")
            )
            .set_required(true)
        )
        /*.add_component(
            dpp::component()
            .set_label("Files")
            .set_type(dpp::cot_file_upload)
            .set_id("files")
            .set_min_values(1)
            .set_max_values(10)
            .set_required(true)
        );*/
        .add_component(
            dpp::component()
            .set_label("Files")
            .set_description("upload ut to 10 files.")
                
            .set_type(dpp::cot_file_upload)
            .set_id("files")
                
            .set_min_values(1)
            .set_max_values(10)
            .set_required(true)
        );

        _event.dialog(modal);

    
    }
}