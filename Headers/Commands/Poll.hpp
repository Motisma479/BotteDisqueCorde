#pragma once
#include "Commands/ICommand.hpp"
#include "PollManager/PollManager.hpp"

namespace Commands
{
	class Poll : public ICommand
	{
	public:
		Poll(const char* _name, dpp::cluster& bot, Data& data, PollManager& manager);

		void Init(bool registerCommand, uint64_t _commandId) override;
		void Execute(const dpp::slashcommand_t& event) override;
	private:
		PollManager& pollManager;
	};
}