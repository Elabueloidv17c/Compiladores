#pragma once
#include "Headers.h"
#include "LexicState.h"

namespace Compiler
{
	class LexAnalyzer;

	class ArithmeticState : public LexicState
	{

		std::string m_buffer;

	public:

		void Read(const char* source, LexAnalyzer& analyzer);

		ArithmeticState(const char* source, LexAnalyzer& analyzer);
		~ArithmeticState();
	};
}