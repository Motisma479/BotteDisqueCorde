#pragma once
#include "Commands/ICommand.hpp"

namespace Commands
{
	class Stop : public ICommand
	{
	public:
		Stop(dpp::cluster& bot, Data& data);

		void Init(bool registerCommand);
		void Execute(const dpp::slashcommand_t& event);
	};
}