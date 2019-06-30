#include "pch.h"
#include "SyntaxMain.h"
#include "SyntaxVar.h"
#include "LexAnalyzer.h"
#include "SyntaxAnalyzer.h"

bool Compiler::SyntaxMain::EofError(std::string description)
{
	if (IsEof())
	{
		m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekPrevToken()->GetLine(),
			msclr::interop::marshal_as<String^>(description), msclr::interop::marshal_as<String^>("Got-> 'eof'"));

		m_syntaxAnalyzer->EraseState();
		return true;
	}

	return false;
}

void Compiler::SyntaxMain::CheckSyntax()
{
	if (EofError("Error: Expected '('"))
	{
		return;
	}
	if (!m_isBlockReached)
	{
		if (!IsEof() && !m_lexAnalyzer->PeekCurrentToken()->GetLexem().compare("("))
		{
			m_lexAnalyzer->GetNextToken();
			if (!IsEof() && !m_lexAnalyzer->PeekCurrentToken()->GetLexem().compare(")"))
			{
				m_lexAnalyzer->GetNextToken();
				if (!IsEof() && !m_lexAnalyzer->PeekCurrentToken()->GetLexem().compare("{"))
				{
					m_lexAnalyzer->GetNextToken();
					while (!IsEof() && m_lexAnalyzer->PeekCurrentToken()->GetLexem() != "}")
					{
						m_isBlockReached = true;

						if (EofError("Error: Expected 'var' or expression"))
						{
							return;
						}
						if (!m_lexAnalyzer->PeekCurrentToken()->GetLexem().compare("var"))
						{
							m_syntaxAnalyzer->AddState(new SyntaxVar(m_lexAnalyzer, m_syntaxAnalyzer, Main_Scope));
							return;
						}
						else
						{
							m_lexAnalyzer->GetNextToken();
						}
					}
					if (EofError("Error: Expected '}'"))
					{
						return;
					}

					m_lexAnalyzer->GetNextToken();
					ExitState();
					return;
				}
				else
				{
					if (EofError("Error: Expected '{'"))
					{
						return;
					}

					std::string token = "Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'";
					m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
					"Error: Expected '{'", msclr::interop::marshal_as<String^>(token));

					ExitState();
					return;
				}
			}
			else
			{
				if (EofError("Error: Expected ')'"))
				{
					return;
				}

				std::string token = "Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'";
				m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
				"Error: Expected ')'", msclr::interop::marshal_as<String^>(token));

				ExitState();
				return;
			}
		}
		else
		{
			std::string token = "Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'";
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
			"Error: Expected '('", msclr::interop::marshal_as<String^>(token));

			ExitState();
			return;
		}
	}
	else
	{
		while (!IsEof() && m_lexAnalyzer->PeekCurrentToken()->GetLexem() != "}")
		{
			if (EofError("Error: Expected 'var' or expression"))
			{
				return;
			}
			if (!m_lexAnalyzer->PeekCurrentToken()->GetLexem().compare("var"))
			{
				m_syntaxAnalyzer->AddState(new SyntaxVar(m_lexAnalyzer, m_syntaxAnalyzer, Main_Scope));
				return;
			}
			else
			{
				m_lexAnalyzer->GetNextToken();
			}
		}
		if (EofError("Error: Expected '}'"))
		{
			return;
		}

		ExitState();
		return;
	}
}

void Compiler::SyntaxMain::ExitState()
{
	Token* currentToken;
	
	if (!IsEof())
	{
		currentToken = m_lexAnalyzer->PeekCurrentToken();
	}

	//Continue until the current expresion ends and go back to begin state
	while (!IsEof() && currentToken->GetLexem() != "}" && currentToken->GetLexem() != "var")
	{
		m_lexAnalyzer->GetNextToken();
		if (EofError("Error: main function declaration"))
		{
			return;
		}
		currentToken = m_lexAnalyzer->PeekCurrentToken();
	}

	if (!IsEof() && currentToken->GetLexem() == "}")
	{
		//Skip the last "}" and go back to previous state
		m_lexAnalyzer->GetNextToken();
	}
	m_syntaxAnalyzer->EraseState();
}

bool Compiler::SyntaxMain::IsEof()
{
	if (m_lexAnalyzer->GetTokenIteratior() == m_lexAnalyzer->GetNumTokens())
	{
		return true;
	}

	return false;
}

Compiler::SyntaxMain::SyntaxMain(LexAnalyzer* lexic, SyntaxAnalyzer* syntax) : SyntaxState(lexic, syntax) 
{
	m_isBlockReached = false;
}

Compiler::SyntaxMain::~SyntaxMain()
{
}
