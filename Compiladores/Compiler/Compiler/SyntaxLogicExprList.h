#pragma once
#include "SyntaxState.h"

namespace Compiler
{
	class SyntaxLogicExprList : public SyntaxState
	{
		std::string				m_scope;
		bool							m_isLastLogicExpr;

	public:

		void							CheckSyntax();

		void							EofError(std::string description);
		void							PanicMode();

		SyntaxLogicExprList(LexAnalyzer* lexic, SyntaxAnalyzer* syntax, std::string scope);
		~SyntaxLogicExprList();
	};
}