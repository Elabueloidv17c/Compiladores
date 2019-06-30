#include "pch.h"
#include "Headers.h"
#include "Compiler.h"

Compiler::Manager::Manager()
{
	m_errorHandler = gcnew ErrorModule();
	m_lexAnalyzer = new LexAnalyzer(m_errorHandler);
	m_symbolTable = new SymbolTable(m_errorHandler);
	m_syntaxAnalyzer = new SyntaxAnalyzer(m_lexAnalyzer, m_errorHandler, m_symbolTable);
	m_semanticAnalyzer = new SemanticAnalyzer(m_errorHandler);
}

Compiler::Manager::~Manager()
{
	if (m_lexAnalyzer != nullptr)
	{
		delete m_lexAnalyzer;
		m_lexAnalyzer = nullptr;
	}
	if (m_syntaxAnalyzer != nullptr)
	{
		delete m_syntaxAnalyzer;
		m_syntaxAnalyzer = nullptr;
	}
	if (m_semanticAnalyzer != nullptr)
	{
		delete m_semanticAnalyzer;
		m_semanticAnalyzer = nullptr;
	}
}

void Compiler::Manager::LexAnalysis(String^ sourceCode)
{
	if(m_lexAnalyzer != nullptr)
	{
		m_lexAnalyzer->ParseSourceCode((const char*) System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(sourceCode).ToPointer());
	}
}

void Compiler::Manager::SyntaxAnalysis()
{
	if (m_lexAnalyzer != nullptr)
	{
		m_syntaxAnalyzer->CheckSyntax();
	}
}

cli::array<String^>^ Compiler::Manager::CompileProgram(String^ sourceCode)
{
	LexAnalysis(sourceCode);
	SyntaxAnalysis();

	std::vector <Token>* tokens = m_lexAnalyzer->GetTokens();
	std::vector <std::string> symbols = m_symbolTable->GetStrSymbols();
	int size = symbols.size();

	//Check for errors
	unsigned short errors = m_lexAnalyzer->GetErrorModule()->GetErrorNumber();

	cli::array<String^>^ compilationDetails = gcnew cli::array<String^>(3 + tokens->size() + errors + symbols.size());



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
		for (index; index < tokens->size(); index++)
		{
			compilationDetails[index + 1] = msclr::interop::marshal_as<String^>(std::to_string(tokens[0][index].GetLine())
				+ '\r' + tokens[0][index].GetLexem() + '\r' + tokens[0][index].GetTypeStr());
		}

		//Add separator
		compilationDetails[++index] = msclr::interop::marshal_as<String^>("@");

		//Add Symbols
		index++;
		for (unsigned int i = 0; i < symbols.size(); i++)
		{
			compilationDetails[index++] = msclr::interop::marshal_as<String^>(symbols[i]);
		}

		//Add separator
		compilationDetails[index++] = msclr::interop::marshal_as<String^>("@");

		//Add Errors
		for (unsigned int i = 0; i < errors; i++)
		{
			compilationDetails[index++] = m_errorHandler->GetErrors()[i];
		}

		tokens->clear();
		m_errorHandler->ClearErrors();
		m_lexAnalyzer->SetCurrentChar(0);
		m_lexAnalyzer->SetCurrentLine(1);
		m_lexAnalyzer->SetCurrentToken(0);
		m_syntaxAnalyzer->ClearStates();
		m_syntaxAnalyzer->SetMainAdded(false);
		m_symbolTable->Clear();

		return compilationDetails;
}



