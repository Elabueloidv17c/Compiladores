#include "pch.h"

#include "Compiler.h"

Compiler::Manager::Manager()
{
	m_errorHandler = gcnew ErrorModule();
	m_lexAnalyzer = new LexAnalyzer(m_errorHandler);
}

Compiler::Manager::~Manager()
{
	if (m_lexAnalyzer != nullptr)
	{
		delete m_lexAnalyzer;
		m_lexAnalyzer = nullptr;
	}
}

void Compiler::Manager::LexAnalysis(String^ sourceCode)
{
	if(m_lexAnalyzer != nullptr)
	{
		m_lexAnalyzer->ParseSourceCode((const char*) System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(sourceCode).ToPointer());
	}
}

cli::array<String^>^ Compiler::Manager::CompileProgram(String^ sourceCode)
{
	cli::array<String^>^ compilationDetails;

	LexAnalysis(sourceCode);

	compilationDetails = gcnew cli::array<String^>(1);
	compilationDetails[0] = gcnew String("---------Compilation Succeeded---------");

	return compilationDetails;
}


