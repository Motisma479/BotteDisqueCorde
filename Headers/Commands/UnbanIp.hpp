#pragma once
#include "Commands/ICommand.hpp"

namespace Commands
{
	class UnbanIp : public ICommand
	{
	public:
		UnbanIp(const char* _name, dpp::cluster& bot, Data& data);

		void Init(CommandIds _commandIds) override;
		void Execute(const dpp::slashcommand_t& _event) override;
	private:
		bool isValidIPv4(const std::string& ip) const;
	};
}