#pragma once

struct Date
{
	int day = -1;
	int month = -1;
	int year = -1;
	int hour = -1;
	int minute = -1;

	int dayOfWeek = -1;
};

inline bool operator >= (const Date& dateA, const Date& dateB)
{
	return dateA.day >= dateB.day && dateA.month >= dateB.month && dateA.year >= dateB.year && dateA.hour >= dateB.hour && dateA.minute >= dateB.minute;
}