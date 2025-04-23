#pragma once
#include "Poll.hpp"
class PollManager
{
public:
	PollManager(){}
	void Add(PollData _poll, uint64_t guildId, uint64_t channelId);
	void Update();
	void Load();
private:
	void Save();
	std::vector<PollData> polls;
};