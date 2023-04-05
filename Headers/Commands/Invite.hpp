#pragma once
#include "Commands/ICommand.hpp"

namespace Commands
{
	class Invite : public ICommand
	{
	public:
		Invite(dpp::cluster& bot, Data& data) : ICommand(bot, data) {}

		void Init();
		void Execute(const dpp::slashcommand_t& event);
	};
}