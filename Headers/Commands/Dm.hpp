#pragma once

#include "Commands/ICommand.hpp"

namespace Commands
{
	class Dm : public ICommand
	{
	public:
		Dm(dpp::cluster& bot, Data& data);
		void Init(bool registerCommand);
		void Execute(const dpp::slashcommand_t& event);
	};
}