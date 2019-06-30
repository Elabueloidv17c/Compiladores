#pragma once
#include "Headers.h"
#include "SyntaxState.h"

namespace Compiler
{
	class LexAnalyzer;
	class SyntaxAnalyzer;

	class SyntaxVar : public SyntaxState
	{
		std::vector <VarData>			m_names;
		SymbolCategory					m_category;
		std::string						m_dataType;
		std::string						m_scope;

	public:

		bool							EofError(std::string description);
		bool							IsEof();
		void							CheckSyntax();
		void							ExitState();
		
		SyntaxVar(LexAnalyzer* lexic, SyntaxAnalyzer* syntax, std::string scope);
		~SyntaxVar();
	};
}
