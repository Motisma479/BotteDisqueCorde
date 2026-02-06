#include "Commands/UnbanIp.hpp"
#include "advancedFunction.hpp"

Commands::UnbanIp::UnbanIp(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data) {}

void Commands::UnbanIp::Init(CommandIds _commandIds)
{
    if (dpp::run_once<struct register_bot_commands>())
    {
        commands.chatCommand = new dpp::slashcommand(name, "Allow a super admin to unban an ip from the hosting server!", cp_bot.me.id);
        commands.chatCommand->add_option(dpp::command_option(dpp::co_string, "ip", "the ipv4 to unban", true)).set_default_permissions(dpp::p_administrator);

        ICommand::Init(_commandIds);
    }
}

void Commands::UnbanIp::Execute(const dpp::slashcommand_t& _event)
{
    ICommand::Execute(_event);
    if (_event.command.get_command_name() == name) {
        if (!advanced::CheckSuperAdminID(_event.command.get_issuing_user().id))
        {
            _event.reply(dpp::message("Only super admin can use that command.").set_flags(dpp::m_ephemeral));
            return;
        }

        std::string ip = std::get<std::string>(_event.get_parameter("ip"));
        if (!isValidIPv4(ip))
        {
            _event.reply(dpp::message("Invalide IP please retry.").set_flags(dpp::m_ephemeral));
            return;
        }

#ifdef _WIN32
        _event.reply(dpp::message("Temporarily not supported.").set_flags(dpp::m_ephemeral));
#else
        _event.reply(dpp::message("IP: \"" + ip + "\" unbaned.").set_flags(dpp::m_ephemeral));
        std::system(std::string("sudo fail2ban-client set sshd unbanip " + ip).c_str());
#endif

    }
}


bool Commands::UnbanIp::isValidIPv4(const std::string& ip) const
{
    int parts = 0;
    int value = 0;
    bool hasDigit = false;

    for (size_t i = 0; i <= ip.size(); ++i) {
        char c = (i < ip.size()) ? ip[i] : '.';

        if (c == '.') {
            if (!hasDigit || value > 255)
                return false;

            parts++;
            value = 0;
            hasDigit = false;
        }
        else if (c >= '0' && c <= '9') {
            hasDigit = true;
            value = value * 10 + (c - '0');
            if (value > 255)
                return false;
        }
        else {
            return false;
        }
    }

    return parts == 4;
}
