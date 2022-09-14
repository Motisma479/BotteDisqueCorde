#include "advancedFunction.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

bool advanced::CheckSuperAdminID(const uint64_t id)
{
	std::ifstream AdmId;
	AdmId.open("superAdminId");
	for (std::string line; std::getline(AdmId, line); )
	{
		if (line.size() <= 1) continue;
		else if (!line.compare(std::to_string(id)))
		{
			return true;
		}
	}
	return false;
}