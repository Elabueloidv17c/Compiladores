#pragma once
#include "SyntaxState.h"

namespace Compiler
{
	class SyntaxMain : public SyntaxState
	{
	public:

		void							EofError(std::string description);
		void							CheckSyntax();
		void							PanicMode();

		SyntaxMain(LexAnalyzer* lexic, SyntaxAnalyzer* syntax);
		~SyntaxMain();
	};
}