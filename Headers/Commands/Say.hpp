#pragma once

#include "Commands/ICommand.hpp"

namespace Commands
{
	class Say : public ICommand
	{
	public:
		Say(dpp::cluster& bot, Data& data);
		void Init(bool registerCommand);
		void Execute(const dpp::slashcommand_t& event);
	};
}