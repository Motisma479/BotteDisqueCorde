#pragma once
#include <dpp/dpp.h>
#include "Initialiser.hpp"

struct CommandIds
{
	uint64_t chat = 0;
	uint64_t message = 0;
	uint64_t user = 0;
};
struct CommandsData
{
	dpp::slashcommand* chatCommand = nullptr;
	dpp::slashcommand* messageCommand = nullptr;
	dpp::slashcommand* userCommand = nullptr;
};

namespace Commands
{
	class ICommand
	{
	public:
		inline ICommand(const char* _name, dpp::cluster& _bot, Data& _data) : name(_name), cp_bot(_bot), cp_data(_data), commandIds({0,0,0}) {}
		
		inline ~ICommand()
		{
			if (commands.chatCommand)
				delete commands.chatCommand;
			if (commands.messageCommand)
				delete commands.messageCommand;
			if (commands.userCommand)
				delete commands.userCommand;
		}

		inline virtual void Init(CommandIds _commandIds)
		{
			commandIds = _commandIds;
			if (commandIds.chat == 0 && commands.chatCommand)
			{
				cp_bot.global_command_create(*commands.chatCommand, [&](const dpp::confirmation_callback_t& cct) { commandIds.chat = cct.get<dpp::slashcommand>().id; });
			}
			if (commandIds.message == 0 && commands.messageCommand)
			{
				cp_bot.global_command_create(*commands.messageCommand, [&](const dpp::confirmation_callback_t& cct) { commandIds.message = cct.get<dpp::slashcommand>().id; });
			}
			if (commandIds.user == 0 && commands.userCommand)
			{
				cp_bot.global_command_create(*commands.userCommand, [&](const dpp::confirmation_callback_t& cct) { commandIds.user = cct.get<dpp::slashcommand>().id; });
			}
		}
		
		//execute on chat
		inline virtual void Execute(const dpp::slashcommand_t& _event) {}
		//execute on message
		inline virtual void Execute(const dpp::message_context_menu_t& _event) {}
		//execute on user
		inline virtual void Execute(const dpp::user_context_menu_t& _event) {}
		
		inline virtual void Reload(bool reRegister)
		{
			if (reRegister)
			{
				if (commands.chatCommand)
				{
					commands.chatCommand->id = commandIds.chat;
					cp_bot.global_command_edit(*commands.chatCommand);
				}
				if (commands.messageCommand)
				{
					commands.messageCommand->id = commandIds.message;
					cp_bot.global_command_edit(*commands.messageCommand);
				}
				if (commands.userCommand)
				{
					commands.userCommand->id = commandIds.user;
					cp_bot.global_command_edit(*commands.userCommand);
				}
			}
		}
		
		std::string name;
	protected:
		dpp::cluster& cp_bot;
		Data& cp_data;
		CommandsData commands;//stock the command so it can be hard reset in runtime;
		CommandIds commandIds;
	};
}