#pragma once
#include "Headers.h"

namespace Compiler
{
	class LexAnalyzer;
	class SyntaxAnalyzer;

	class SyntaxState
	{
	protected:

		LexAnalyzer*			m_lexAnalyzer;
		SyntaxAnalyzer*		m_syntaxAnalyzer;

	public:

		virtual void CheckSyntax();
		virtual void EofError(std::string description);
		virtual void PanicMode();

		SyntaxState(LexAnalyzer* lexic, SyntaxAnalyzer* syntax);
		virtual ~SyntaxState();
	};
}