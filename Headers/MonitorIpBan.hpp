#pragma once
#include <dpp/dpp.h>
#include <Initialiser.hpp>

class MonitorIpBan
{
public:
	MonitorIpBan(dpp::cluster& _bot, Data& _data);
	void Init();
	void Monitor();//must be called in a loop

private:
	int iInit = 0;
	char buffer[1024];
	dpp::cluster& cp_bot;
	Data& cp_data;
};