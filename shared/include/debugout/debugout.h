/*
* Custom ostringstream class that outputs to the debugger console on Windows.
*/
#ifndef NDEBUG
#ifndef DEBUG_OUT_H
#define DEBUG_OUT_H

#include <sstream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <iostream>
#endif

// Create a temporary that outputs stream data on destruction.
#define DEBUG_OUT DebugOut()
class DebugOut : public std::ostringstream {
public:
	~DebugOut() {
	#ifdef _WIN32
		OutputDebugString(this->str().c_str());
	#else
		std::cout << this->str();
	#endif
	}
};
#endif
#endif