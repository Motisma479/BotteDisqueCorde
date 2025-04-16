#pragma once
#include "Commands/ICommand.hpp"
#include <random>

namespace Commands
{
	class Meme : public ICommand
	{
	public:
		Meme(dpp::cluster& bot, Data& data);

		void Init(bool registerCommand);
		void Execute(const dpp::slashcommand_t& event);
		void Reload();
	private:
		std::uniform_int_distribution<int> memeGenerator;
		std::mt19937 rng;
		int lastRandom = -1;
	};
}