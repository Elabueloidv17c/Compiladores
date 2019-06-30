#pragma once
#include "Headers.h"
#include "SyntaxState.h"

namespace Compiler
{
	class SyntaxParameter : public SyntaxState
	{
		SymbolCategory					m_category;
		std::string						m_dataType;
		std::string						m_function;
		std::vector <VarData>			m_names;

	public:

		void							CheckSyntax();
		void							ExitState();
		bool							EofError(std::string description);
		bool							IsEof();

		SyntaxParameter(LexAnalyzer* lexic, SyntaxAnalyzer* syntax, std::string function);
		~SyntaxParameter();
	};
}

