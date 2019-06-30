#pragma once
#include "windows.h"
#include "vcclr.h"
#include "Headers.h"
#include "ErrorModule.h"
#include "LexAnalyzer.h"
#include "SymbolTable.h"


namespace Compiler
{
	class SyntaxState;
	
	class SyntaxAnalyzer
	{
		msclr::gcroot<ErrorModule^>					m_errorModule;
		std::stack <SyntaxState*>					m_states;

		SymbolTable*								m_symbolTable;
		LexAnalyzer*								m_lexic;

		bool										m_isMainAdded;

	public:

		SymbolTable*								GetSymbolTable();
		void										AddState(SyntaxState* state);
		void										CheckSyntax();
		void										EraseState();
		void										ClearStates();

		void										SetMainAdded(bool state);
		bool										GetMainStatus();

		SyntaxAnalyzer(LexAnalyzer* lexic, ErrorModule^ errors, SymbolTable* symbols);
		~SyntaxAnalyzer();
	};
}

