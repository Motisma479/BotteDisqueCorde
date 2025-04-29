#pragma once
#include "Commands/ICommand.hpp"

namespace Commands
{
	class Reload : public ICommand
	{
	public:
		Reload(const char* _name, dpp::cluster& bot, Data& data, std::vector<std::unique_ptr<Commands::ICommand>>& CommandList);

		void Init(bool registerCommand, uint64_t _commandId) override;
		void Execute(const dpp::slashcommand_t& event) override;
	private:
		std::vector<std::unique_ptr<Commands::ICommand>>& CommandList;
	};
}