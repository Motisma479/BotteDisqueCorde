#pragma once
#include "Commands/ICommand.hpp"

namespace Commands
{
	class Reload : public ICommand
	{
	public:
		Reload(dpp::cluster& bot, Data& data, std::vector<std::unique_ptr<Commands::ICommand>>& CommandList);

		void Init();
		void Execute(const dpp::slashcommand_t& event);
	private:
		std::vector<std::unique_ptr<Commands::ICommand>>& CommandList;
	};
}