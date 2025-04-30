#include "MonitorIpBan.hpp"

MonitorIpBan::MonitorIpBan(dpp::cluster& _bot, Data& _data) : cp_bot(_bot), cp_data(_data)
{
#if(WIN32)
#else
    log.open("/var/log/fail2ban.log", std::ios::in);
    if (!log.is_open()) {
        std::cerr << "Failed to open log file!" << std::endl;
        return;
    }
    // Set the position to the end of the file initially
    log.seekg(0, std::ios::end);
    last_pos = log.tellg();
    log.close;
#endif
}

void MonitorIpBan::Monitor()
{
#if(WIN32)
    return;
#endif
    log.open("/var/log/fail2ban.log", std::ios::in);
    if (!log.is_open()) {
        std::cerr << "Failed to open log file!" << std::endl;
        return;
    }
    log.seekg(last_pos);
    std::string line;
    std::vector<std::string> messages;

    while (std::getline(log, line)) {
        // Process the log line if it contains the "Ban" keyword
        if (line.find("Ban ") != std::string::npos) {
            size_t ip_pos = line.find("Ban ") + 4;  // Move past "Ban "
            std::string ip = line.substr(ip_pos, line.find(" ", ip_pos) - ip_pos);
            messages.push_back( "**IP: " + ip + "**\n-# Banned for 24h.**" );
        }
    }

    last_pos = log.tellg();
    log.close();

    if (messages.size() == 0) return;


    for (const uint64_t& id : cp_data.GetIpBanListner())
    {
        std::shared_ptr<size_t> index = std::make_shared<size_t>(0);
        std::function<void(const dpp::confirmation_callback_t& callback)> sendRecursive = [=](const dpp::confirmation_callback_t& callback) mutable
            {
                (*index)++;
                if (*index >= messages.size()) return;

                cp_bot.message_create(dpp::message(id, messages[*index]), sendRecursive);
            };
        cp_bot.message_create(dpp::message(id, messages[*index]), sendRecursive);
    }

    messages.clear();
}
