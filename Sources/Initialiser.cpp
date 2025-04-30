#include "Initialiser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

const std::string GetToken()
{
	std::ifstream token;
	token.open("token.ini");
	for (std::string line; std::getline(token, line); )
	{
		std::istringstream stream(line);
		std::string param;
		stream >> param;
		if (param.size() <= 1) continue;
		else if (!param.compare("BOT_TOKEN:"))
		{
			std::cout << "token loaded" << std::endl;
			std::string test;
			stream >> test;
			return test;
		}
	}
}

Data::Data()
{
	std::ifstream settings;
	settings.open("settings.ini");
	for (std::string line; std::getline(settings, line); )
	{
		std::istringstream stream(line);
		std::string param;
		stream >> param;
		if (param.size() <= 1) continue;
		else if (!param.compare("STOP_THE_MACHINE_ON_COMMAND:"))
		{
			std::string test;
			stream >> test;
			if (!test.compare("true"))
			{
				stopMachine = true;
				continue;
			}
			stopMachine = false;
		}
		else if (!param.compare("PRESSENCE_MESSAGE:"))
		{
			std::string message;
			std::getline(stream,message);
			if (!message.empty() && message[1] == '\"' && message.back() == '\"')
			{
				message = message.substr(2, message.length() - 3);
			}

			pressenceMessage = message;
		}
		else if (!param.compare("IP_BAN_LISTNER:"))
		{
			std::string message;
			while (std::getline(stream, message, ','))
			{
				ipBanListner.push_back(std::stoull(message));
			}
		}
		//else if (!param.compare("MAX_SUS_IMAGES:"))
		//	stream >> maxSusImages;
	}
	
	

	std::filesystem::path p = std::filesystem::current_path().append("Resources/AMOGUS");
	for (const auto& entry : std::filesystem::directory_iterator(p))
	{
		std::cout << entry.path() << std::endl;
		AMOGUSFiles.push_back(entry.path().string());
	}
	p = std::filesystem::current_path().append("Resources/MEME");
	for (const auto& entry : std::filesystem::directory_iterator(p))
	{
		std::cout << entry.path() << std::endl;
		memeFiles.push_back(entry.path().string());
	}

	

	std::cout << "data loaded" << std::endl;
}

Data::~Data()
{
}

void Data::Reload()
{
	AMOGUSFiles.clear();
	memeFiles.clear();

	std::filesystem::path p = std::filesystem::current_path().append("Resources/AMOGUS");
	for (const auto& entry : std::filesystem::directory_iterator(p))
	{
		std::cout << entry.path() << std::endl;
		AMOGUSFiles.push_back(entry.path());
	}
	p = std::filesystem::current_path().append("Resources/MEME");
	for (const auto& entry : std::filesystem::directory_iterator(p))
	{
		std::cout << entry.path() << std::endl;
		memeFiles.push_back(entry.path());
	}
}

const std::vector<std::filesystem::path>& Data::GetSusImages()
{
	return AMOGUSFiles;
}
const std::vector<std::filesystem::path>& Data::GetMemeImages()
{
	return memeFiles;
}

const bool& Data::GetStopMachine()
{
	return stopMachine;
}

const std::string& Data::GetPressenceMessage()
{
	return pressenceMessage;
}
void Data::SetPressenceMessage(std::string message)
{
	pressenceMessage = message;
	Save();
}

const std::vector<uint64_t>& Data::GetIpBanListner()
{
	return ipBanListner;
}
bool Data::AddIpBanListner(uint64_t _newChannel)
{
	for (int i = 0; i < ipBanListner.size();i++)
	{
		if (ipBanListner[i] == _newChannel)
		{
			ipBanListner.erase(ipBanListner.begin() + i);
			Save();
			return false;
		}
	}
	ipBanListner.push_back(_newChannel);
	Save();
	return true;
}

void Data::Save()
{
	std::ofstream file("settings.ini");
	if (file.is_open())
	{
		file << "STOP_THE_MACHINE_ON_COMMAND: " << (stopMachine ? "true" : "false") << '\n';

		file << "PRESSENCE_MESSAGE: \"" << pressenceMessage << "\"\n";

		if (ipBanListner.size() > 0)
		{
			file << "IP_BAN_LISTNER: ";
			for (int i = 0; i < ipBanListner.size() - 1;i++)
				file << ipBanListner[i] << ',';
			file << ipBanListner.back();
		}

		file.close();
	}
}