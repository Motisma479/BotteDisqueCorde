#pragma once

#include "Commands/ICommand.hpp"

namespace Commands
{
	class Say : public ICommand
	{
	public:
		Say(const char* _name, dpp::cluster& bot, Data& data);
		void Init(bool registerCommand, uint64_t _commandId);
		void Execute(const dpp::slashcommand_t& event);
	};
}