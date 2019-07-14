#pragma once
#include "Headers.h"
#include "SyntaxState.h"

namespace Compiler
{
	class SyntaxIf : public SyntaxState
	{
		std::string				m_scope;
		bool							m_isReturningIf;
		bool							m_isReturningBlock;

	public:

		void							CheckSyntax();

		void							EofError(std::string description);
		void							PanicMode();

		SyntaxIf(LexAnalyzer* lexic, SyntaxAnalyzer* syntax, std::string scope);
		~SyntaxIf();
	};
}