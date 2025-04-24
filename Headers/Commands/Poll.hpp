#pragma once
#include "Commands/ICommand.hpp"
#include "PollManager/PollManager.hpp"

namespace Commands
{
	class Poll : public ICommand
	{
	public:
		Poll(dpp::cluster& bot, Data& data, PollManager& manager);

		void Init(bool registerCommand);
		void Execute(const dpp::slashcommand_t& event);
	private:
		PollManager& pollManager;
	};
}