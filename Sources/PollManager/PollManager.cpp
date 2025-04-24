#include "PollManager/PollManager.hpp"
#include "advancedFunction.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <codecvt>

PollManager::PollManager(dpp::cluster& _bot): bot(_bot)
{ 
	oldDate = advanced::GetActualDate();
	Load();
	std::cout << "PollManager Loaded\n";
}
void PollManager::Add(PollData _poll, uint64_t channelId)
{
	polls.push_back(std::make_tuple(_poll, channelId));
	needSave = true;
}
int PollManager::Delete(uint16_t _pollId, uint64_t _channelId)
{
	for (int i = 0; i < polls.size(); i++)
	{
		const auto& [pollData, channelId] = polls[i];
		if (pollData.id == _pollId)
		{
			if (channelId == _channelId)
			{
				deleteList.push_back(i);
				needSave = true;
				return 0;
			}
			return 2;//poll access denied
		}
	}
	return 1;//no poll found
}
void PollManager::Update()
{
	if (frame < updateFrame) //update every updateFrame
	{
		frame++;
		return;
	}
	frame = 0;

	Date actualDate = advanced::GetActualDate();

	if (saveHour > actualDate.hour) saveHour = actualDate.hour;
	if (needSave && saveHour < actualDate.hour)
	{
		Save();
		needSave = false;
		saveHour = actualDate.hour;
	}

	for (int i = 0; i < deleteList.size(); i++)
	{
		polls.erase(polls.begin() + deleteList[i]);
	}
	deleteList.clear();

	if (oldDate >= actualDate) return;
	oldDate = actualDate;

	for (const auto& [pollData, channelId] : polls)
	{
		Date pollDate = pollData.GetDueDate();
		if (pollData.shouldRepeat)
		{
			if (actualDate.dayOfWeek == pollDate.dayOfWeek &&
				actualDate.hour == pollDate.hour &&
				actualDate.minute == pollDate.minute)
			{
				Send(pollData, channelId);
			}
		}
		else
		{
			if (actualDate >= pollDate)
			{
				Send(pollData, channelId);
				Delete(pollData.id, channelId);
			}
		}
	}
}
std::vector<PollData> PollManager::List(uint64_t _channelId)
{
	std::vector<PollData> temp;
	for (const auto& [pollData, channelId] : polls)
	{
		if (channelId == _channelId) temp.push_back(pollData);
	}
	return temp;
}
void PollManager::Load()
{
	std::ifstream file("Poll.ini");
	dpp::snowflake chanelId;
	PollData data;
	bool first = true;

	int i = 0;

	for (std::string line; std::getline(file, line); )
	{
		std::istringstream stream(line);
		std::string param;
		stream >> param;
		
		std::string p = param;

		if (param.size() <= 1) continue;
		else if (param.starts_with("[poll:"))
		{
			i++;
			if (first)
			{
				first = false;
				continue;
			}
			polls.push_back(std::make_tuple(data, chanelId));
			data.answers.clear();
		}
		else if (param.starts_with("pollId="))
		{
			std::string temp = param.substr(7);

			int value = std::stoi(temp);
			if (value < 0 || value > UINT16_MAX) {
				throw std::out_of_range("Value out of int16_t range");
			}

			data.id = static_cast<uint16_t>(value);
		}
		else if (param.starts_with("channelId="))
		{
			std::string temp = param.substr(10);

			chanelId = dpp::snowflake(temp);
		}
		else if (param.starts_with("pollQuestion="))
		{
			std::string l; std::getline(stream, l);
			std::string temp = param.substr(13) +l;

			data.title = temp;
		}
		else if (param.starts_with("pollDuration="))
		{
			std::string temp = param.substr(13);

			data.duration = std::stod(temp);
		}
		else if (param.starts_with("pollDate="))
		{
			std::string l; std::getline(stream,l);
			std::string temp = param.substr(9) + l;
					
			data.SetDueDate(advanced::ParseDateTime(temp));
		}
		else if (param.starts_with("pollRepeat="))
		{
			std::string temp = param.substr(11);

			if (temp.size() > 1) continue;
					
			data.shouldRepeat = temp[0] == '1';
		}
		else if (param.starts_with("answer="))
		{
			std::string l; std::getline(stream, l);
			std::string temp = param.substr(7) +l;

			data.answers.push_back(std::make_tuple(temp,""));
		}
		else if (param.starts_with("emoji="))
		{
			std::string l; std::getline(stream, l);
			std::string temp = param.substr(6) + l;

			if (data.answers.size() == 0) continue;
			std::get<1>(data.answers.back()) = temp;
		}
	}
	if(i != polls.size())
		polls.push_back(std::make_tuple(data, chanelId));
}

void PollManager::Save()
{
	std::ofstream file("Poll.ini");
	if (file.is_open()) {
	
		int i = 0;
		for (const auto& [pollData, channelId] : polls)
		{
			file << "[poll:" << std::to_string(i) << "]\n";
			file << "pollId=" << pollData.id << '\n';
			file << "channelId=" << channelId << '\n';
			file << "pollQuestion=" << pollData.title << '\n';
			file << "pollDuration=" << std::to_string(pollData.duration) << '\n';
			Date d = pollData.GetDueDate();
			file << "pollDate=" << d.year << '-' << (d.month < 10 ? ("0" + std::to_string(d.month)) : std::to_string(d.month)) << '-' << (d.day < 10 ? ("0" + std::to_string(d.day)) : std::to_string(d.day)) << ' ' << (d.hour < 10 ? ("0" + std::to_string(d.hour)) : std::to_string(d.hour)) << ':' << (d.minute < 10 ? ("0" + std::to_string(d.minute)) : std::to_string(d.minute)) << '\n';
			file << "pollRepeat=" << (pollData.shouldRepeat ? '1' : '0') << '\n';
			for (const auto& [answer, emoji] : pollData.answers )
			{
				file << "answer=" << answer << '\n';
				if (emoji.size() > 0)
					file << "emoji=" << emoji << '\n';
			}
			file << '\n';
			i++;
		}
		file.close();
	}
}

void PollManager::Send(const PollData& data, const dpp::snowflake& id)
{
	dpp::poll poll;
	poll.question.text = data.title;
	poll.expiry = data.duration;
	for (const auto [answer, emoji] : data.answers)
	{
		dpp::emoji temp;

		std::regex emoji_regex(R"(<a?:([a-zA-Z0-9_]+):(\d+)>)");
		std::smatch match;
		if (std::regex_match(emoji, match, emoji_regex)) {
			temp.name = match[1];
			temp.id = dpp::snowflake(std::string(match[2]));
			if (emoji[1] == 'a')
				temp.flags |= dpp::e_animated;
		}
		else
		{
			temp.name = emoji;
		}

		poll.add_answer(answer, temp);
	}
	bot.message_create(dpp::message(id, "").set_poll(poll));
}