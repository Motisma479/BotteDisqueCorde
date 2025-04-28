#pragma once
#include "Commands/ICommand.hpp"
#include <random>

namespace Commands
{
	class Dice : public ICommand
	{
	public:
		Dice(const char* _name, dpp::cluster& bot, Data& data);

		void Init(bool registerCommand, uint64_t _commandId);
		void Execute(const dpp::slashcommand_t& event);
	private:
		std::mt19937 rng;
	};
}