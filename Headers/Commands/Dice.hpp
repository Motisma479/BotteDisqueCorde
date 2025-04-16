#pragma once
#include "Commands/ICommand.hpp"
#include <random>

namespace Commands
{
	class Dice : public ICommand
	{
	public:
		Dice(dpp::cluster& bot, Data& data);

		void Init(bool registerCommand);
		void Execute(const dpp::slashcommand_t& event);
	private:
		std::mt19937 rng;
	};
}