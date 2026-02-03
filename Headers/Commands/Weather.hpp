#pragma once
#include "Commands/ICommand.hpp"

namespace Commands
{
	class Weather : public ICommand
	{
	public:
		Weather(const char* _name, dpp::cluster& bot, Data& data);

		void Init(CommandIds _commandIds) override;
		void Execute(const dpp::slashcommand_t& event) override;
	private:
        std::string GetWindDirPoint(int _degrees);
        std::string GetEmojiFromWeatherId(int _id);
	};
}