#pragma once
#include "SyntaxState.h"

namespace Compiler
{
	class SyntaxFunctionCall : public SyntaxState
	{
		std::string				m_scope;
		bool							m_isReturning;

	public:

		void							CheckSyntax();

		void							EofError(std::string description);
		void							PanicMode();

		SyntaxFunctionCall(LexAnalyzer* lexic, SyntaxAnalyzer* syntax, std::string scope);
		~SyntaxFunctionCall();
	};
}