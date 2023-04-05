#pragma once
#include "Commands/ICommand.hpp"
#include <random>

namespace Commands
{
	class Amogus : public ICommand
	{
	public:
		Amogus(dpp::cluster& bot, Data& data);

		void Init();
		void Execute(const dpp::slashcommand_t& event);
		void Reload();
	private:
		std::uniform_int_distribution<int> AMOGUSGenerator;
		std::mt19937 rng;
		int lastRandom = -1;
	};
}