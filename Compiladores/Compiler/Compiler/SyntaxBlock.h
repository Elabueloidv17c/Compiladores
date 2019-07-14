#pragma once
#include "Headers.h"
#include "SyntaxState.h"

namespace Compiler
{
	class LexAnalyzer;
	class SyntaxAnalyzer;

	class SyntaxBlock : public SyntaxState
	{
		std::string				m_scope;
		bool							m_isReturning;
	public:

		void							EofError(std::string description);
		void							CheckSyntax();
		void							PanicMode();

		SyntaxBlock(LexAnalyzer* lexic, SyntaxAnalyzer* syntax, std::string scope);
		~SyntaxBlock();
	};
}
