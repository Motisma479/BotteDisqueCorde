#include "advancedFunction.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>

bool advanced::CheckSuperAdminID(const uint64_t id)
{
	std::ifstream AdmId;
	AdmId.open("superAdminId");
	for (std::string line; std::getline(AdmId, line); )
	{
		if (line.size() <= 1) continue;
		else if (!line.compare(std::to_string(id)))
		{
			return true;
		}
	}
	return false;
}

date advanced::GetActualDate()
{
	date result;

	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);

	result.day = now->tm_mday;
	result.dayOfWeek = now->tm_wday;

	result.month = now->tm_mon + 1;

	result.year = now->tm_year + 1900;

	result.hour = now->tm_hour;

	result.minute = now->tm_min;
	return result;
}