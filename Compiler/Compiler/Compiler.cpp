#include "pch.h"
#include "Headers.h"
#include "Compiler.h"

Compiler::Manager::Manager()
{
	m_errorHandler = gcnew ErrorModule();
	m_lexAnalyzer = new LexAnalyzer(m_errorHandler);
	m_tokens = new std::vector<Token>;
}

Compiler::Manager::~Manager()
{
	if (m_lexAnalyzer != nullptr)
	{
		delete m_lexAnalyzer;
		m_lexAnalyzer = nullptr;
	}

	if (m_tokens != nullptr)
	{
		m_tokens->clear();
		delete m_tokens;
		m_tokens = nullptr;
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
	LexAnalysis(sourceCode);
	m_tokens = m_lexAnalyzer->GetTokens();

	//Check for errors
	unsigned short errors = m_lexAnalyzer->GetErrorModule()->GetErrorNumber();

	cli::array<String^>^ compilationDetails = gcnew cli::array<String^>(2 + m_tokens->size() + errors);

	unsigned long index = 0;

	//Add compilation state in the first index
	if (!m_errorHandler->GetErrorNumber())
	{
		compilationDetails[index] = gcnew String("---------Compilation Succeeded---------");
	}
	else
	{
		compilationDetails[index] = gcnew String("----------Compilation Failed----------");
	}

	//Add tokens
	for (index; index < m_tokens->size(); index++)
	{
		compilationDetails[index + 1] = msclr::interop::marshal_as<String^>(std::to_string(m_tokens[0][index].GetLine())
		+ "#" + m_tokens[0][index].GetLexem() + "#" + m_tokens[0][index].GetTypeStr());
	}

	//Add separator
	compilationDetails[++index] = msclr::interop::marshal_as<String^>("@");

	//Add Errors
	index++;
	for (unsigned int i = 0; i < errors; i++)
	{
		compilationDetails[index++] = m_errorHandler->GetErrors()[i];
	}

	m_tokens->clear();
	m_errorHandler->ClearErrors();
	m_lexAnalyzer->SetCurrentChar(0);
	m_lexAnalyzer->SetCurrentLine(1);

	return compilationDetails;
}


