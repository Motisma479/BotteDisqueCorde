#pragma once
#include "Commands/ICommand.hpp"

namespace Commands
{
	class Clear : public ICommand
	{
	public:
		Clear(dpp::cluster& bot, Data& data);
		void Init();
		void Execute(const dpp::slashcommand_t& event);
	};
}