#pragma once
#include "Commands/ICommand.hpp"
#include <random>

namespace Commands
{
	class Meme : public ICommand
	{
	public:
		Meme(const char* _name, dpp::cluster& bot, Data& data);

		void Init(bool registerCommand, uint64_t _commandId);
		void Execute(const dpp::slashcommand_t& event);
		void Reload();
	private:
		std::uniform_int_distribution<int> memeGenerator;
		std::mt19937 rng;
		int lastRandom = -1;
	};
}