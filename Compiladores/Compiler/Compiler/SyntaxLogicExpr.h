#pragma once
#include "Headers.h"
#include "SyntaxState.h"

namespace Compiler
{
	class SyntaxLogicExpr : public SyntaxState
	{

		std::string				m_scope;

		int								m_close;
		int								m_open;

		bool							m_isExitSet;
		bool							m_isCondition;

		bool							m_isTermFound;
		bool							m_isOperatorFound;
		bool							m_isLastInList;

		void							ProcessTerm();
		void							ProcessNot();

	public:

		void							EofError(std::string description);
		void							PanicMode();
		void							CheckSyntax();


		SyntaxLogicExpr(LexAnalyzer* lexic, SyntaxAnalyzer* syntax, std::string scope, bool isCondition);
		~SyntaxLogicExpr();
	};
}