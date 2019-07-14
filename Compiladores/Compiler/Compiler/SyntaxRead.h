#pragma once
#include "Headers.h"
#include "SyntaxState.h"

namespace Compiler
{
	class SyntaxRead : public SyntaxState
	{
		std::string				m_scope;
		bool							m_isReturning;
		bool							m_isDimension;

	public:

		void							CheckSyntax();

		void							EofError(std::string description);
		void							PanicMode();


		SyntaxRead(LexAnalyzer* lexic, SyntaxAnalyzer* syntax, std::string scope);
		~SyntaxRead();
	};
}