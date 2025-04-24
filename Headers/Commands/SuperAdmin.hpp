#pragma once
#include "Commands/ICommand.hpp"

namespace Commands
{
	class SuperAdmin : public ICommand
	{
	public:
		SuperAdmin(dpp::cluster& bot, Data& data);

		void Init(bool registerCommand);
		void Execute(const dpp::slashcommand_t& event);
	};
}