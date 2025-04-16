#pragma once
#include "Commands/ICommand.hpp"

namespace Commands
{
	class Ping : public ICommand
	{
	public:
		Ping(dpp::cluster& bot, Data& data);

		void Init(bool registerCommand);
		void Execute(const dpp::slashcommand_t& event);
	};
}