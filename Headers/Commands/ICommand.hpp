#pragma once
#include <dpp/dpp.h>
#include "Initialiser.hpp"

namespace Commands
{
	class ICommand
	{
	public:
		inline ICommand(dpp::cluster& bot, Data& data) : cp_bot(bot), cp_data(data) {}
		inline virtual void Init() {}
		inline virtual void Execute(const dpp::slashcommand_t& event) {}
		inline virtual void Reload() {}
	protected:
		dpp::cluster& cp_bot;
		Data& cp_data;
	};
}