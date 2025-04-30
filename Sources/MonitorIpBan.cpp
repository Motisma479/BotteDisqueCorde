#include "MonitorIpBan.hpp"

MonitorIpBan::MonitorIpBan(dpp::cluster& _bot, Data& _data) : cp_bot(_bot), cp_data(_data) {}

#if(WIN32)
void MonitorIpBan::Init()
{

}

void MonitorIpBan::Monitor()
{

}
#else
#include <sys/inotify.h>
void MonitorIpBan::Init()
{
	iInit = inotify_init();
    if (fd == -1)
    {
        std::cerr << "Failed to initialize inotify" << std::endl;
        return;
    }

    int wd = inotify_add_watch(fd, "/var/log/fail2ban.log", IN_MODIFY);
    if (wd == -1)
    {
        std::cerr << "Failed to add inotify watch on file" << std::endl;
        close(fd);
        return;
    }
}

void MonitorIpBan::Monitor()
{
    int length = read(fd, buffer, sizeof(buffer));
    if (length < 0) {
        std::cerr << "Error reading inotify events" << std::endl;
        close(fd);
        return;
    }

    for (int i = 0; i < length; i += sizeof(struct inotify_event) + ((struct inotify_event*)&buffer[i])->len) {
        struct inotify_event* event = (struct inotify_event*)&buffer[i];

        // Check if the file was modified
        if (event->mask & IN_MODIFY)
        {
            std::ifstream log(/ var / log / fail2ban.log);
            std::string line;

            while (std::getline(log, line)) {
                if (line.find("Ban ") != std::string::npos) {
                    size_t ip_pos = line.find("Ban ") + 4; // Move past "Ban "
                    std::string ip = line.substr(ip_pos, line.find(" ", ip_pos) - ip_pos);
                    std::string message = "**IP " + ip + " has been banned for 24h**";

                    for(const uint64_t& id : cp_data.GetIpBanListner())
                        cp_bot.message_create(dpp::message(id,message));
                }
            }
            log.close();
        }
    }
}
#endif