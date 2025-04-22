#include "PollManager/Poll.hpp"
#include <chrono>

PollData::PollData(std::string _title, double _duration, Date _date)
{
    title = _title;
    duration = _duration;
    dueDate = _date;
    dueDate.dayOfWeek = GetWeekDay(_date.day, _date.month, _date.year);
}

PollData::PollData(std::string _title, double _duration)
{
    title = _title;
    duration = _duration;
}

int PollData::GetWeekDay(int day, int month, int year)
{
    std::tm time_in;
    time_in.tm_year = year - 1900;
    time_in.tm_mon = month - 1;
    time_in.tm_mday = day;

    std::mktime(&time_in);

    return time_in.tm_wday;
}

void PollData::SetDueDate(Date _dueDate)
{
    dueDate = _dueDate;
    if(dueDate.dayOfWeek == -1)
        dueDate.dayOfWeek = GetWeekDay(_dueDate.day, _dueDate.month, _dueDate.year);
}

Date PollData::GetDueDate()
{
    return dueDate;
}