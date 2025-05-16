#pragma once
#include <dpp/dpp.h>
#include "Initialiser.hpp"
#include "advancedFunction.hpp"

struct IPInfo
{
	std::string country;
	std::string region;
	std::string city;
	unsigned int banCount = 0;
	Date refreshDate;
};

class MonitorIpBan
{
public:
	MonitorIpBan(dpp::cluster& _bot, Data& _data);
	void Monitor();//must be called in a loop

private:
	dpp::cluster& cp_bot;
	Data& cp_data;

	std::vector<std::tuple<std::string, IPInfo>> info;

	std::streampos last_pos = 0;
	std::ifstream log;

	std::vector<std::string> messages;
	
	void MessageViaRequest(std::string ip, IPInfo* data = nullptr);

	void Save();
	void Load();
};