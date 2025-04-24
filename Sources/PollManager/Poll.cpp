#include "PollManager/Poll.hpp"
#include <chrono>

int16_t counter = 0;

PollData::PollData(std::string _title, double _duration, Date _date)
{
    title = _title;
    duration = _duration;
    dueDate = _date;
    dueDate.dayOfWeek = GetWeekDay(_date.day, _date.month, _date.year);


    auto now = std::chrono::system_clock::now().time_since_epoch();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
    id = static_cast<uint16_t>((ms % 30000) + (counter++ % 500));
}

PollData::PollData(std::string _title, double _duration)
{
    title = _title;
    duration = _duration;

    auto now = std::chrono::system_clock::now().time_since_epoch();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
    id = static_cast<uint16_t>((ms % 30000) + (counter++ % 500));
}

PollData::PollData()
{

}

int PollData::GetWeekDay(int day, int month, int year)
{
    std::tm time_in = {};
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

Date PollData::GetDueDate() const
{
    return dueDate;
}