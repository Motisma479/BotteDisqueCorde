#pragma once
#include <vector>

struct Poll
{
public:
	std::string title;
	double duration;
	std::vector < std::tuple<std::string, std::string> awnsers;

	

	bool shouldRepeat;
};