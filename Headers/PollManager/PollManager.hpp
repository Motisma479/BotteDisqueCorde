#pragma once
#include <cstdint>

#include "Poll.hpp"
#include "dpp.h"
class PollManager
{
public:
	PollManager(dpp::cluster& _bot);
	void Add(PollData _poll, uint64_t channelId);
	int Delete(uint16_t _pollId, uint64_t _channelId);
	std::vector<PollData> List(uint64_t _channelId);
	void Update();
private:
	void Load();
	void Save();
	void Send(const PollData& data, const dpp::snowflake& id);
	bool needSave = false;
	int saveHour = 0;
	std::vector<int> deleteList;

	int updateFrame = 20;
	int frame = 0;
	Date oldDate;

	dpp::cluster& bot;
	std::vector<std::tuple<PollData, dpp::snowflake>> polls;
};