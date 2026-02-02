#pragma once
#include "Commands/ICommand.hpp"
#include <random>

namespace Commands
{
	class Guess : public ICommand
	{
	public:
		Guess(const char* _name, dpp::cluster& bot, Data& data);

		void Init(CommandIds _commandIds) override;
		void Execute(const dpp::slashcommand_t& event) override;
	private:
		std::string wordToGuess;
		bool asWrongChar = false;
		std::array<bool, 24> wrongChars;

		static std::vector<uint64_t> blackID;
		static std::vector<uint64_t> greenID;
		static std::vector<uint64_t> yellowID;
	};
}