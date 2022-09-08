#include "Initialiser.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

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