//=====================================================
// ConsoleCommands.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_ConsoleCommands__
#define __included_ConsoleCommands__

#include <vector>

class ConsoleArguments{
public:
	const std::string* m_args;

	inline ConsoleArguments(const std::string* args) :m_args(args){}
};


typedef bool(*console_command_t)(ConsoleArguments* args);

class ConsoleCommand{
public:
	ConsoleCommand(const char* name, console_command_t command);

	bool RunCommand(ConsoleArguments *args){
		return m_command(args);
	}

	const char* m_name;
	console_command_t m_command;
};
extern std::vector<ConsoleCommand*>* s_theCommandList;


bool RunConsoleCommand(const std::string& name, const std::string* args);
bool RunCommand(const std::string& name, ConsoleArguments* args);
bool RunCommandFromLine(const std::string& line);


//Register a new function
#define CONSOLE_COMMAND(name) static bool ConsoleCommand_##name##(ConsoleArguments* arg); \
static ConsoleCommand command_##name##(#name, ConsoleCommand_##name##); \
static bool ConsoleCommand_##name##(ConsoleArguments* args)

//Register already existing function
#define COMMAND_REGISTER(name) static ConsoleCommand ConsoleCommand_##name##(#name, name)


//Command list for running command line arguments
typedef std::vector<std::string> CommandList;
CommandList ParseCommands(const std::string& command_line);
bool ProcessCommands(const CommandList& commands);

#endif