#pragma once
#include "Commands/ICommand.hpp"

namespace Commands
{
	class Invite : public ICommand
	{
	public:
		Invite(const char* _name, dpp::cluster& bot, Data& data);

		void Init(CommandIds _commandIds) override;
		void Execute(const dpp::slashcommand_t& event) override;
	};
}