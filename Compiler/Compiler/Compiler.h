#pragma once
#include "windows.h"
#include "ErrorModule.h"
#include "LexAnalyzer.h"

using namespace System;

namespace Compiler 
{
	public ref class Manager
	{
		LexAnalyzer*		m_lexAnalyzer;
		ErrorModule^		m_errorHandler;

		void LexAnalysis(String^ sourceCode);

	public:

		Manager();
		~Manager();
		
		cli::array<String^>^ CompileProgram(String^ sourceCode);
	};
}
