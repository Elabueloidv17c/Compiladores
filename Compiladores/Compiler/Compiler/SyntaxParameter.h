#pragma once
#include "Headers.h"
#include "SyntaxState.h"

namespace Compiler
{
	class SyntaxParameter : public SyntaxState
	{
		SymbolCategory					m_category;
		std::string							m_dataType;
		std::string							m_function;
		std::vector <VarData>		m_names;
		bool										m_isRepeating;

	public:

		void							CheckSyntax();
		void							PanicMode();
		void							EofError(std::string description);

		SyntaxParameter(LexAnalyzer* lexic, SyntaxAnalyzer* syntax, std::string function);
		~SyntaxParameter();
	};
}

