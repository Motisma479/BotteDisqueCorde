#pragma once

#include "Commands/ICommand.hpp"

namespace Commands
{
	class Settings : public ICommand
	{
	public:
		Settings(const char* _name, dpp::cluster& bot, Data& data);
		void Init(CommandIds _commandIds) override;
		void Execute(const dpp::slashcommand_t& _event) override;
	};
}