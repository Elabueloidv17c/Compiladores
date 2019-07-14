#pragma once
#include "SyntaxState.h"

namespace Compiler
{
	class SyntaxPrint : public SyntaxState
	{
		std::string				m_scope;
		bool							m_isReturning;

	public:

		void							CheckSyntax();

		void							EofError(std::string description);
		void							PanicMode();

		SyntaxPrint(LexAnalyzer* lexic, SyntaxAnalyzer* syntax, std::string scope);
		~SyntaxPrint();
	};
}