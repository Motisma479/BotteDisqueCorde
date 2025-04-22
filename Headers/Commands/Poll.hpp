#pragma once
#include "Commands/ICommand.hpp"

namespace Commands
{
	class Poll : public ICommand
	{
	public:
		Poll(dpp::cluster& bot, Data& data);

		void Init(bool registerCommand);
		void Execute(const dpp::slashcommand_t& event);
	};
}