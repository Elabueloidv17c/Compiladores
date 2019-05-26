#pragma once
#include "Headers.h"
#include "LexicState.h"

namespace Compiler
{
	class LexAnalyzer;

	class IntState : public LexicState
	{
		std::string m_buffer;

	public:

		void Read(const char* source, LexAnalyzer& analyzer);

		IntState(bool isNegative);
		~IntState();
	};
}

