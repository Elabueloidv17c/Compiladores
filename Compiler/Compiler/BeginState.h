#pragma once
#include "LexicState.h"

namespace Compiler
{
	class LexAnalyzer;

	class BeginState : public LexicState
	{
	public:

		void Read(const char* source, LexAnalyzer& analyzer);

		BeginState();
		~BeginState();
	};
}

