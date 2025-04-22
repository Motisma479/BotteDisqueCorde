#pragma once
#include <vector>
#include <string>
#include "Date.hpp"

struct PollData
{
public:
	PollData(std::string _title, double _duration, Date _date);
	PollData(std::string _title, double _duration);
	std::string title;
	double duration;
	std::vector<std::tuple<std::string, std::string>> awnsers;

	void SetDueDate(Date _dueDate);
	Date GetDueDate();
	bool shouldRepeat;
private:
	Date dueDate;
	int GetWeekDay(int _day, int _month, int _year);
};