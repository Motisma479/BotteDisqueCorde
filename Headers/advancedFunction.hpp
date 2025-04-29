#include <stdint.h>
#include <iostream>
#include "PollManager/Date.hpp"

namespace advanced
{
	bool CheckSuperAdminID(const uint64_t id);
	Date GetActualDate();
    Date ParseDateTime(const std::string& _input);

	//presise up to 30 min for more presision create new emoji
	std::string GetActualClockEmoji();
}