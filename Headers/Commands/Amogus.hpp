#pragma once
#include "Commands/ICommand.hpp"
#include <random>

namespace Commands
{
	class Amogus : public ICommand
	{
	public:
		Amogus(const char* _name, dpp::cluster& bot, Data& data);

		void Init(bool registerCommand, uint64_t _commandId) override;
		void Execute(const dpp::slashcommand_t& event) override;
		void Reload(bool reRegister) override;
	private:
		std::uniform_int_distribution<int> AMOGUSGenerator;
		std::mt19937 rng;
		int lastRandom = -1;
	};
}