#include "pch.h"
#include "SyntaxFunction.h"
#include "SyntaxBegin.h"
#include "SyntaxVar.h"
#include "SyntaxMain.h"
#include "LexAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "Token.h"

Compiler::SyntaxBegin::SyntaxBegin(LexAnalyzer* lexic, SyntaxAnalyzer* syntax) : SyntaxState(lexic, syntax)
{
	m_isMainAdded = false;
}

Compiler::SyntaxBegin::~SyntaxBegin()
{

}

void Compiler::SyntaxBegin::CheckSyntax()
{
	if (IsEof())
	{
		if (!m_syntaxAnalyzer->GetMainStatus())
		{
			if (m_lexAnalyzer->GetNumTokens())
			{
				m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekPrevToken()->GetLine(),
				"Error: main function not found", "");
				return;
			}
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, 1, "Error: main function not found", "");
		}
		return;
	}

	Token* currentToken = m_lexAnalyzer->PeekCurrentToken();

	//Transitions
	if (!currentToken->GetLexem().compare("var"))
	{
		m_syntaxAnalyzer->AddState(new SyntaxVar(m_lexAnalyzer, m_syntaxAnalyzer, Global_Scope));
		return;
	}
	else if (!currentToken->GetLexem().compare("function"))
	{
		m_syntaxAnalyzer->AddState(new SyntaxFunction(m_lexAnalyzer, m_syntaxAnalyzer));
		return;
	}
	else if (!currentToken->GetLexem().compare("main"))
	{
		if (m_syntaxAnalyzer->GetMainStatus())
		{
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
			"Error: main function already exist", "");

			PanicMode();
			return;
		}

		m_syntaxAnalyzer->SetMainAdded(true);
		m_syntaxAnalyzer->AddState(new SyntaxMain(m_lexAnalyzer, m_syntaxAnalyzer));
		return;
	}
	else
	{
		m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(), "Error: Expected 'main', 'function' or 'var'", 
		msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));

		PanicMode();
	}
}

void Compiler::SyntaxBegin::PanicMode()
{
	Token* currentToken = m_lexAnalyzer->PeekCurrentToken();

	while (currentToken->GetLexem() != ";" && currentToken->GetLexem() != "var" && currentToken->GetLexem() != "function"
	&& currentToken->GetLexem() != "main")
	{
		m_lexAnalyzer->GetNextToken();
		if (IsEof())
		{
			return;
		}
		currentToken = m_lexAnalyzer->PeekCurrentToken();
	}
	if (!IsEof())
	{
		if (m_lexAnalyzer->PeekCurrentToken()->GetLexem() == ";")
		{
			m_lexAnalyzer->GetNextToken();
		}
	}
}

bool Compiler::SyntaxBegin::GetMainStatus()
{
	return m_isMainAdded;
}

bool Compiler::SyntaxBegin::IsEof()
{
	if (m_lexAnalyzer->GetTokenIteratior() == m_lexAnalyzer->GetNumTokens())
	{
		return true;
	}

	return false;
}