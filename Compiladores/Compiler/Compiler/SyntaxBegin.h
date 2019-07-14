#pragma once
#include "SyntaxState.h"

namespace Compiler
{
	class LexAnalyzer;
	class SyntaxAnalyzer;

	class SyntaxBegin : public SyntaxState
	{
		bool m_isMainAdded;

	public:

		bool GetMainStatus();
		void CheckSyntax();
		void PanicMode();

		SyntaxBegin(LexAnalyzer* lexic, SyntaxAnalyzer* syntax);
		~SyntaxBegin();
	};
}