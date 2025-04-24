#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include "Date.hpp"

struct PollData
{
public:
	PollData(std::string _title, double _duration, Date _date);
	PollData(std::string _title, double _duration);
	PollData();
	std::string title;
	double duration;
	std::vector<std::tuple<std::string, std::string>> answers;

	void SetDueDate(Date _dueDate);
	Date GetDueDate() const;
	bool shouldRepeat = false;

	uint16_t id;
private:
	Date dueDate;
	int GetWeekDay(int _day, int _month, int _year);
};