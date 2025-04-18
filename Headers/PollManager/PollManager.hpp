#pragma once
#include "Poll.hpp"
class PollManager
{
public:
	void Add(Poll _poll);
	void Update();
	void Load();
private:
	void Save();
	std::vector<Poll> polls;
};