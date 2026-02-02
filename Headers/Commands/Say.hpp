#pragma once

#include "Commands/ICommand.hpp"

namespace Commands
{
	class Say : public ICommand
	{
	public:
		Say(const char* _name, dpp::cluster& bot, Data& data);
		void Init(CommandIds _commandIds) override;
		void Execute(const dpp::slashcommand_t& event) override;
	};
}