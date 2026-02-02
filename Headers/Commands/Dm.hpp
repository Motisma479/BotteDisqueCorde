#pragma once

#include "Commands/ICommand.hpp"

namespace Commands
{
	class Dm : public ICommand
	{
	public:
		Dm(const char* _name, dpp::cluster& bot, Data& data);
		void Init(CommandIds _commandIds) override;
		void Execute(const dpp::slashcommand_t& event) override;
		void Execute(const dpp::user_context_menu_t& _event) override;
	};
}