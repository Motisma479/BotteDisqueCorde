#pragma once
#include "Commands/ICommand.hpp"

namespace Commands
{
	class ListenToBanIp : public ICommand
	{
	public:
		ListenToBanIp(const char* _name, dpp::cluster& bot, Data& data);

		void Init(bool registerCommand, uint64_t _commandId) override;
		void Execute(const dpp::slashcommand_t& event) override;
	};
}