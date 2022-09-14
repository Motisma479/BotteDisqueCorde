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
		else if (!param.compare("MAX_SUS_IMAGES:"))
			stream >> maxSusImages;
	}
	std::cout << "data loaded" << std::endl;
}

Data::~Data()
{
}

int Data::getMaxSusImages()
{
	return maxSusImages;
}