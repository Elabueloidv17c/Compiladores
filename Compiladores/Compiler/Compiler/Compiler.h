#pragma once
#include "windows.h"
#include "ErrorModule.h"
#include "LexAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "SemanticAnalyzer.h"
#include "SymbolTable.h"

using namespace System;

namespace Compiler 
{
	public ref class Manager
	{
		ErrorModule^			m_errorHandler;
		SymbolTable*			m_symbolTable;

		LexAnalyzer*			m_lexAnalyzer;
		SyntaxAnalyzer*			m_syntaxAnalyzer;
		SemanticAnalyzer*		m_semanticAnalyzer;

		void LexAnalysis(String^ sourceCode);
		void SyntaxAnalysis();

	public:

		Manager();
		~Manager();
		
		cli::array<String^>^ CompileProgram(String^ sourceCode);
	};
}
