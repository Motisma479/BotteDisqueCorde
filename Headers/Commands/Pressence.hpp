#pragma once
#include "Commands/ICommand.hpp"

namespace Commands
{
	class Pressence : public ICommand
	{
	public:
		Pressence(dpp::cluster& bot, Data& data);

		void Init(bool registerCommand);
		void Execute(const dpp::slashcommand_t& event);
	};
}