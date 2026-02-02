#include "Commands/Ping.hpp"

Commands::Ping::Ping(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data) {}

void Commands::Ping::Init(CommandIds _commandIds)
{
    if (dpp::run_once<struct register_bot_commands>())
    {
        commands.chatCommand = new dpp::slashcommand(name, "Ping pong!", cp_bot.me.id);
        commands.chatCommand->set_dm_permission(true);

        ICommand::Init(_commandIds);
    }
}

void Commands::Ping::Execute(const dpp::slashcommand_t& event)
{
    ICommand::Execute(event);
    if (event.command.get_command_name() == name) {
        double latency_ms = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch() ).count() - static_cast<long long>(event.command.get_creation_time() * 1000.0);
        auto api_start = std::chrono::steady_clock::now();
       
        std::string temp = "**:ping_pong:Pong!**\n▬▬▬▬▬▬▬▬▬▬\n> **latency:**\n> " + std::to_string(latency_ms) + " **ms**" + "\n▬▬▬▬▬▬▬▬▬▬\n> **API:**\n> <a:loading:1362431721399783505>";
        event.reply(temp, [event, latency_ms, api_start](const dpp::confirmation_callback_t& callback) {
            auto api_end = std::chrono::steady_clock::now();
            double api_ms = std::chrono::duration_cast<std::chrono::milliseconds>(api_end - api_start).count();
            
            std::string after = "**:ping_pong:Pong!**\n▬▬▬▬▬▬▬▬▬▬\n> **latency:**\n> " + std::to_string(latency_ms) + " **ms**" + "\n▬▬▬▬▬▬▬▬▬▬\n> **API:**\n> " + std::to_string(api_ms) + " **ms**";;
            event.edit_original_response(dpp::message(after));
        });
        /*
        bool isOnline = true;
        std::string starName = "S_000001";
        double ping = 1.53;
        std::string originPath = "C:\\Users\\robio\\Dev";
        std::string currentPath = "C:\\Users\\robio";

        dpp::message msg;
        msg.flags |= dpp::m_using_components_v2;

        msg.add_component_v2(
            dpp::component()
            .set_type(dpp::cot_container)
            .set_accent(0x482B99)
            .add_component_v2(
                dpp::component()
                .set_type(dpp::cot_section)
                .add_component_v2(
                    dpp::component()
                    .set_type(dpp::cot_text_display)
                    .set_content("# " + starName)
                )
                .add_component_v2(
                    dpp::component()
                    .set_type(dpp::cot_text_display)
                    .set_content("▬▬▬▬▬▬▬▬")
                )
                .add_component_v2(
                    dpp::component()
                    .set_type(dpp::cot_text_display)
                    .set_content("## Status:")
                )
                .set_accessory(
                    dpp::component()
                    .set_type(dpp::cot_thumbnail)
                    .set_description("A cat")
                    .set_thumbnail("https://i.imgur.com/mB8LUdC.png")
                )
            )
            .add_component_v2(
                dpp::component()
                .set_type(dpp::cot_text_display)
                .set_content(">>> " + std::string(isOnline ? ":green_circle: **Online**" : ":red_circle: **Offline**") + "\n**Ping:** *" + std::to_string(ping) + "ms*")
            )
            .add_component_v2(
                dpp::component()
                .set_type(dpp::cot_text_display)
                .set_content("**Origin Path:**\n```bash\n" + originPath + "\n```")
            )
            .add_component_v2(
                dpp::component()
                .set_type(dpp::cot_text_display)
                .set_content("**Current Path:**\n```bash\n" + currentPath + "\n```")
            )
            .add_component_v2(
                dpp::component()
                .set_type(dpp::cot_action_row)
                .add_component_v2(
                    dpp::component()
                    .set_type(dpp::cot_button)
                    .set_style(dpp::cos_primary)
                    .set_id("star_newcommand")
                    .set_label("New Command")
                    .set_disabled(!isOnline)
                )
                .add_component_v2(
                    dpp::component()
                    .set_type(dpp::cot_button)
                    .set_style(dpp::cos_danger)
                    .set_id("star_shutdown")
                    .set_label("Shutdown")
                    .set_disabled(!isOnline)
                )
            )
            
        );
        event.reply(msg);
        */
    }
}