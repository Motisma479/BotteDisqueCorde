#pragma once
#include "Commands/ICommand.hpp"

namespace Commands
{
	class Invite : public ICommand
	{
	public:
		Invite(dpp::cluster& bot, Data& data);

		void Init(bool registerCommand);
		void Execute(const dpp::slashcommand_t& event);
	};
}