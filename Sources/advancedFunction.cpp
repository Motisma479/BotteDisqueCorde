#include "advancedFunction.hpp"
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

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

Date advanced::GetActualDate()
{
	Date result;

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

Date advanced::ParseDateTime(const std::string& _input)
{
	std::tm temp = {};
	std::istringstream ss(_input);
	ss >> std::get_time(&temp, "%Y-%m-%d %H:%M");

	if (ss.fail()) {
		throw std::runtime_error("Failed to parse datetime string");
	}

	Date result;
	result.day = temp.tm_mday;
	result.month = temp.tm_mon + 1;
	result.year = temp.tm_year + 1900;

	result.hour = temp.tm_hour;
	result.minute = temp.tm_min;

	return result;
}