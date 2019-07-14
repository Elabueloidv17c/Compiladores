#pragma once
#include "Headers.h"
#include "SyntaxState.h"

namespace Compiler
{
	class SyntaxWhile : public SyntaxState
	{
		std::string				m_scope;
		bool							m_isReturning;

	public:

		void							CheckSyntax();

		void							EofError(std::string description);
		void							PanicMode();

		SyntaxWhile(LexAnalyzer* lexic, SyntaxAnalyzer* syntax, std::string scope);
		~SyntaxWhile();
	};
}