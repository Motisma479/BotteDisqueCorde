#include "Initialiser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

const std::string getToken()
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

void Data::reload()
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

const std::vector<std::filesystem::path>& Data::getSusImages()
{
	return AMOGUSFiles;
}
const std::vector<std::filesystem::path>& Data::getMemeImages()
{
	return memeFiles;
}