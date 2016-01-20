// =====================================================
// ConsoleCommands.cpp
// by Andrew Socha
// =====================================================

#include "Engine/Core/EngineCore.hpp"
#include "Engine/Console/ConsoleCommands.hpp"
#include "Engine/Core/Utilities.hpp"

std::vector<ConsoleCommand*>* s_theCommandList = nullptr;


static std::vector<ConsoleCommand*>* GetAllCommands(){ //force things to initialize in the correct order
	if (s_theCommandList == nullptr){
		s_theCommandList = new std::vector<ConsoleCommand*>();
	}
	return s_theCommandList;
}

///=====================================================
/// 
///=====================================================
ConsoleCommand::ConsoleCommand(const char* name, console_command_t command) :
m_name(name),
m_command(command){
	std::vector<ConsoleCommand*>* commandListPtr = GetAllCommands();
	commandListPtr->push_back(this);
}

///=====================================================
/// 
///=====================================================
bool RunConsoleCommand(const std::string& name, const std::string* args){
	ConsoleArguments arg_obj(args);
	return RunCommand(name, &arg_obj);
}

///=====================================================
/// 
///=====================================================
bool RunCommand(const std::string& name, ConsoleArguments* args){
	ConsoleCommand* commandToExec = nullptr;
	for (std::vector<ConsoleCommand*>::const_iterator commandIter = s_theCommandList->cbegin(); commandIter != s_theCommandList->cend(); ++commandIter){
		ConsoleCommand* command = *commandIter;
		if (_stricmp(command->m_name, name.c_str()) == 0){ //case insensitive compare
			commandToExec = command;
			break;
		}
	}
	if (commandToExec == nullptr) {
		return false;
	}

	return commandToExec->RunCommand(args);
}

///=====================================================
/// 
///=====================================================
bool RunCommandFromLine(const std::string& line){
	std::vector<std::string> strings;

	std::string textCopy = line;
	while (!textCopy.empty() && textCopy.back() == ' '){ //remove trailing spaces
		textCopy.pop_back();
	}

	size_t characterIndex = textCopy.find_first_of(' ', 0); //find the name of the command
	if (characterIndex != std::string::npos){
		strings.push_back(textCopy.substr(0, characterIndex));

		textCopy = textCopy.substr(++characterIndex);
		characterIndex = textCopy.find_first_of(' ', 0);
	}

	if (strings.size() == 0){ //command has no arguments
		strings.push_back(textCopy);
		return RunConsoleCommand(strings[0], nullptr);
	}

	strings.push_back(""); //temporary string that will house the number of arguments

	while (characterIndex != std::string::npos){ //find the arguments of the command
		strings.push_back(textCopy.substr(0, characterIndex));

		textCopy = textCopy.substr(++characterIndex);
		characterIndex = textCopy.find_first_of(' ', 0);
	}
	strings.push_back(textCopy);

	strings[1] = std::to_string(strings.size() - 2); //first argument is the number of remaining arguments

	return RunConsoleCommand(strings[0], strings.data() + 1);
}

///==========================================================================================================================================
/// 
///==========================================================================================================================================
CommandList ParseCommands(const std::string& commandLine){
	size_t length = commandLine.size();
	const char* stream = commandLine.data();

	CommandList commands;
	std::string command;
	
	char* token = (char*)stream;
	while (token <= (stream + length) && *token != '-')
		++token;

	while (token <= (stream + length)){
		command = ++token;

		int commandLength = 0;
		while (token <= (stream + length) && *token != '-'){
			++token;
			++commandLength;
		}

		command.resize(commandLength);

		if (!command.empty()) {
			commands.push_back(command);
		}
	}

	return commands;
}

///=====================================================
/// 
///=====================================================
bool ProcessCommands(const CommandList& commands){
	bool didRunCommand = false;
	for (CommandList::const_iterator commandIter = commands.cbegin(); commandIter != commands.cend(); ++commandIter){
		didRunCommand = RunCommandFromLine(*commandIter) || didRunCommand;
	}

	return didRunCommand;
}