#include "advancedFunction.hpp"
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>
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
			AdmId.close();
			return true;
		}
	}
	AdmId.close();
	return false;
}

Date advanced::GetActualDate()
{
	Date result;

	auto now_utc = std::chrono::system_clock::now();
	auto tz = std::chrono::locate_zone("Europe/Paris");
	std::chrono::zoned_time zt{ tz, now_utc };

	auto local_tp = zt.get_local_time();

	auto local_days = floor<std::chrono::days>(local_tp);
	std::chrono::year_month_day ymd{ local_days };
	std::chrono::hh_mm_ss hms{ local_tp - local_days };
	std::chrono::weekday wd{ local_days };

	result.day = unsigned(ymd.day());
	result.dayOfWeek = wd.iso_encoding();

	result.month = unsigned(ymd.month());

	result.year = int(ymd.year());

	result.hour = hms.hours().count();

	result.minute = hms.minutes().count();
	return result;
}

Date advanced::ParseDateTime(const std::string& _input)
{
	std::tm temp = {};
	std::istringstream ss(_input);
	ss >> std::get_time(&temp, "%d-%m-%Y %H:%M");

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

std::string advanced::GetActualClockEmoji()
{
	std::string output = ":clock";
	auto now_utc = std::chrono::system_clock::now();
	auto tz = std::chrono::locate_zone("Europe/Paris");
	std::chrono::zoned_time zt{ tz, now_utc };

	auto local_tp = zt.get_local_time();

	auto local_days = floor<std::chrono::days>(local_tp);
	std::chrono::hh_mm_ss hms{ local_tp - local_days };

	int h = hms.hours().count();
	if (h >= 13) h -= 12;

	output += std::to_string(h);

	int m = hms.minutes().count();

	if (m >= 30) output += "30";

	output += ":";
	return output;
}