#pragma once
#include "Poll.hpp"
class PollManager
{
public:
	void Add(PollData _poll);
	void Update();
	void Load();
private:
	void Save();
	std::vector<PollData> polls;
};