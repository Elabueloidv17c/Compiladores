#pragma once
#include "SyntaxState.h"

namespace Compiler
{
	class SyntaxMain : public SyntaxState
	{
		bool							m_isBlockReached;

	public:

		bool							EofError(std::string description);
		void							CheckSyntax();
		void							ExitState();
		bool							IsEof();

		SyntaxMain(LexAnalyzer* lexic, SyntaxAnalyzer* syntax);
		~SyntaxMain();
	};
}