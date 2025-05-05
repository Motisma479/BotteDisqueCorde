#pragma once
#include <dpp/dpp.h>
#include <Initialiser.hpp>

class MonitorIpBan
{
public:
	MonitorIpBan(dpp::cluster& _bot, Data& _data);
	void Monitor();//must be called in a loop

private:
	dpp::cluster& cp_bot;
	Data& cp_data;

	std::streampos last_pos = 0;
	std::ifstream log;

	std::vector<std::string> messages;
};