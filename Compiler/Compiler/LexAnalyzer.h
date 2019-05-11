#pragma once
#include "Headers.h"
#include "ErrorModule.h"

namespace Compiler
{
	class LexAnalyzer
	{
	public:
		bool ParseSourceCode(const char* source);
		LexAnalyzer(ErrorModule^ errorModule);

		LexAnalyzer();
		~LexAnalyzer();
	};
}