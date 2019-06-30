#include "pch.h"
#include "SyntaxFunction.h"
#include "SyntaxParameter.h"
#include "SyntaxBegin.h"
#include "LexAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "SyntaxVar.h"

bool Compiler::SyntaxFunction::IsEof()
{
	if (m_lexAnalyzer->GetTokenIteratior() == m_lexAnalyzer->GetNumTokens())
	{
		return true;
	}

	return false;
}

void Compiler::SyntaxFunction::CheckSyntax()
{
	if (!m_isParameterReached)
		m_eofMessage = "Error: Expected parameter or ')'";
	else if (m_isParameterReached && !m_isBlockReached)
		m_eofMessage = "Error: Expected ':'";
	else
		m_eofMessage = "Error: Expected 'var' or expression";

	if (EofError(m_eofMessage))
	{
		return;
	}

	Token* currentToken = m_lexAnalyzer->PeekCurrentToken();

	if (!m_isParameterReached)
	{
		if (!IsEof() && m_lexAnalyzer->PeekCurrentToken()->GetType() == TokenType::ID)
		{
			m_name = m_lexAnalyzer->PeekCurrentToken()->GetLexem();
			m_line = m_lexAnalyzer->PeekCurrentToken()->GetLine();

			m_lexAnalyzer->GetNextToken();
			if (EofError("Error: Expected '('"))
			{
				return;
			}
			currentToken = m_lexAnalyzer->PeekCurrentToken();

			if (!currentToken->GetLexem().compare("("))
			{
				m_isParameterReached = true;
				m_syntaxAnalyzer->AddState(new SyntaxParameter(m_lexAnalyzer, m_syntaxAnalyzer, m_name));
				return;
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
			std::string token = "Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'";
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
			"Error: Expected ID", msclr::interop::marshal_as<String^>(token));

			ExitState();
			return;
		}
	}
	if (EofError("Error: Expected ':'"))
	{
		return;
	}
	if (!m_isBlockReached)
	{
		if (!currentToken->GetLexem().compare(":"))
		{
			m_isBlockReached = true;

			m_lexAnalyzer->GetNextToken();
			if (EofError("Error: Expected return value"))
			{
				return;
			}
			currentToken = m_lexAnalyzer->PeekCurrentToken();

			if (currentToken->GetLexem() == "int" || currentToken->GetLexem() == "float" || currentToken->GetLexem() == "string" 
			|| currentToken->GetLexem() == "bool" || currentToken->GetLexem() == "void")
			{
				m_returnValue = currentToken->GetLexem();

				m_syntaxAnalyzer->GetSymbolTable()->AddSymbol(m_line, m_name, m_category, 0, m_returnValue, Global_Scope);
			}
			else
			{
				std::string token = "Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'";
				m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
				"Error: Invalid DataType", msclr::interop::marshal_as<String^>(token));

				ExitState();
				return;
			}

			m_lexAnalyzer->GetNextToken();
			if (EofError("Error: Expected '{'"))
			{
				return;
			}
			currentToken = m_lexAnalyzer->PeekCurrentToken();

			if (currentToken->GetLexem() == "{")
			{
				m_lexAnalyzer->GetNextToken();
				if (EofError("Error: Expected 'var' or expression"))
				{
					return;
				}
				currentToken = m_lexAnalyzer->PeekCurrentToken();
			}
			else
			{
				std::string token = "Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'";
				m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
				"Error: Expected '{'", msclr::interop::marshal_as<String^>(token));
				
				ExitState();
				return;
			}
		}
		else
		{
			std::string token = "Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'";
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
			"Error: Expected Return value", msclr::interop::marshal_as<String^>(token));

			ExitState();
			return;
		}
	}
	else
	{
		if (EofError("Error: Expected '{'"))
		{
			return;
		}
	
		while (!IsEof() && m_lexAnalyzer->PeekCurrentToken()->GetLexem() != "}")
		{
			if (IsEof())
			{
				EofError("Error: Expected 'var' or expression");
				return;
			}
			if (!m_lexAnalyzer->PeekCurrentToken()->GetLexem().compare("var"))
			{
				m_syntaxAnalyzer->AddState(new SyntaxVar(m_lexAnalyzer, m_syntaxAnalyzer, m_name));
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

void Compiler::SyntaxFunction::ExitState()
{
	Token* currentToken = m_lexAnalyzer->PeekCurrentToken();

	//Continue until the current expresion ends and go back to begin state
	while (!IsEof() && currentToken->GetLexem() != "}" && currentToken->GetLexem() != "function" && currentToken->GetLexem() != "main"
	&& currentToken->GetLexem() != "var")
	{
		m_lexAnalyzer->GetNextToken();
		if (EofError("Error: " + m_name + " function declaration"))
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

bool Compiler::SyntaxFunction::EofError(std::string description)
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

Compiler::SyntaxFunction::SyntaxFunction(LexAnalyzer* lexic, SyntaxAnalyzer* syntax) : SyntaxState(lexic, syntax) 
{
	m_isParameterReached = false;
	m_isBlockReached = false;
	m_category = SymbolCategory::Function;
}

Compiler::SyntaxFunction::~SyntaxFunction()
{
}
