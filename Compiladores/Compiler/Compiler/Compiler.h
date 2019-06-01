#pragma once
#include "windows.h"
#include "ErrorModule.h"
#include "LexAnalyzer.h"

using namespace System;

namespace Compiler 
{
	public ref class Manager
	{
		ErrorModule^			m_errorHandler;
		LexAnalyzer*			m_lexAnalyzer;
		
		std::vector <Token>*	m_tokens;

		void LexAnalysis(String^ sourceCode);

	public:

		Manager();
		~Manager();
		
		cli::array<String^>^ CompileProgram(String^ sourceCode);
	};
}
