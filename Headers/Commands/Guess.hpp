#pragma once
#include "Commands/ICommand.hpp"
#include <random>

namespace Commands
{
	class Guess : public ICommand
	{
	public:
		Guess(const char* _name, dpp::cluster& bot, Data& data);

		void Init(bool registerCommand, uint64_t _commandId) override;
		void Execute(const dpp::slashcommand_t& event) override;
	private:
		std::string wordToGuess;
		bool asWrongChar = false;
		std::vector<bool> wrongChars;

		static std::vector<uint64_t> blackID;
		static std::vector<uint64_t> greenID;
		static std::vector<uint64_t> yellowID;
	};
}