#pragma once
#include <dpp/dpp.h>
#include "Initialiser.hpp"

namespace Commands
{
	class ICommand
	{
	public:
		inline ICommand(const char* _name, dpp::cluster& _bot, Data& _data) : name(_name), cp_bot(_bot), cp_data(_data), commandId(0){}
		
		inline virtual void Init(bool _registerCommand, uint64_t _commandId) { commandId = _commandId; }
		
		inline virtual void Execute(const dpp::slashcommand_t& _event) {}
		
		inline virtual void Reload(bool reRegister)
		{
			if (reRegister)
			{
				command.id = commandId;
				cp_bot.global_command_edit(command);
			}
		}
		
		std::string name;
	protected:
		dpp::cluster& cp_bot;
		Data& cp_data;
		dpp::slashcommand command;//stock the command so it can be hard reset in runtime;
		uint64_t commandId;
	};
}