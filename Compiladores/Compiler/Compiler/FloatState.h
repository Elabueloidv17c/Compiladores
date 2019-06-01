#pragma once
#include "Headers.h"
#include "LexicState.h"

namespace Compiler
{
	class LexAnalyzer;

	class FloatState : public LexicState
	{

		std::string m_buffer;

	public:

		void Read(const char* source, LexAnalyzer& analyzer);

		FloatState(const char* source, LexAnalyzer& analyzer);
		FloatState(std::string intPart);
		~FloatState();
	};
}