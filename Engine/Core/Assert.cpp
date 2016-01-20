//=====================================================
// Assert.cpp
// by Andrew Socha
//=====================================================

#include "Assert.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Engine/Console/Console.hpp"
#include "Engine/Sound/SoundSystem.hpp"

///=====================================================
/// 
///=====================================================
void FatalError(const char* file, int line, const std::string& message /*= ""*/){
	ConsolePrintf("\nFATAL ERROR:\n%s(%i)\n%s%s\n", file, line, message.c_str(), message.empty() ? "" : "\n");

	std::string errorMessage = "Fatal error at line " + std::to_string(line) + " in file " + file + ".";
	if (!message.empty()){
		errorMessage += "\n\n" + message;
	}

	MessageBoxA(nullptr, errorMessage.c_str(), "Fatal Error", MB_OK | MB_ICONSTOP);

	if (s_theSoundSystem)
		s_theSoundSystem->Shutdown();

	exit(EXIT_FAILURE);
}

///=====================================================
/// 
///=====================================================
void FatalAssert(const char* file, int line, const char* function) {
	ConsolePrintf("\nFATAL ASSERTION (%s):\n%s(%i)\n\n", function, file, line);

	std::string errorMessage = "Fatal assertion at line " + std::to_string(line) + " in file " + file + ".\n\n" + function;
	int choice = MessageBoxA(nullptr, errorMessage.c_str(), "Fatal Assertion", MB_RETRYCANCEL | MB_ICONSTOP);
	if (choice == IDRETRY){
		__debugbreak();
	}

	if (s_theSoundSystem)
		s_theSoundSystem->Shutdown();

	exit(EXIT_FAILURE);
}

///=====================================================
/// 
///=====================================================
void RecoverableError(const char* file, int line, const std::string& message /*= ""*/) {
	ConsolePrintf("\nRECOVERABLE ERROR:\n%s(%i)\n%s%s\n", file, line, message.c_str(), message.empty() ? "" : "\n");

	std::string errorMessage = "Recoverable error at line " + std::to_string(line) + " in file " + file + ".";
	if (!message.empty()){
		errorMessage += "\n\n" + message;
	}

	int choice = MessageBoxA(nullptr, errorMessage.c_str(), "Recoverable Error", MB_OKCANCEL | MB_ICONEXCLAMATION);
	if (choice == IDCANCEL){
		if (s_theSoundSystem)
			s_theSoundSystem->Shutdown();

		exit(EXIT_FAILURE);
	}
}

///=====================================================
/// 
///=====================================================
void RecoverableAssert(const char* file, int line, const char* function) {
	ConsolePrintf("\nRECOVERABLE ASSERTION (%s):\n%s(%i)\n\n", function, file, line);

	std::string errorMessage = "Recoverable assertion at line " + std::to_string(line) + " in file " + file + ".\n\n" + function;
	int choice = MessageBoxA(nullptr, errorMessage.c_str(), "Recoverable Assertion", MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION);
	if (choice == IDRETRY){
		__debugbreak();
	}
	else if (choice == IDABORT){
		if (s_theSoundSystem)
			s_theSoundSystem->Shutdown();

		exit(EXIT_FAILURE);
	}
}