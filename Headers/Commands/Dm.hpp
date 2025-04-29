#pragma once

#include "Commands/ICommand.hpp"

namespace Commands
{
	class Dm : public ICommand
	{
	public:
		Dm(const char* _name, dpp::cluster& bot, Data& data);
		void Init(bool registerCommand, uint64_t _commandId) override;
		void Execute(const dpp::slashcommand_t& event) override;
	};
}