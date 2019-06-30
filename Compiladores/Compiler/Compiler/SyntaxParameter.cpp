#include "pch.h"
#include "SyntaxParameter.h"
#include "SyntaxBegin.h"
#include "LexAnalyzer.h"
#include "SyntaxAnalyzer.h"

void Compiler::SyntaxParameter::CheckSyntax()
{
	if (EofError("Error: Expected ID or ')'"))
	{
		return;
	}

	if (!m_lexAnalyzer->PeekCurrentToken()->GetLexem().compare(")"))
	{
		//Parameter block is empty
		ExitState();
		return;
	}

	bool isIDFound = false;
	Token* currentToken = m_lexAnalyzer->PeekCurrentToken();

	while (!IsEof())
	{
		if (currentToken->GetType() == TokenType::ID)
		{
			isIDFound = true;

			//Add the current name to the vector
			VarData data;
			data.name = currentToken->GetLexem();
			data.line = currentToken->GetLine();

			m_lexAnalyzer->GetNextToken();
			if (EofError("Error: Expected '[', ':' or ','"))
			{
				return;
			}
			currentToken = m_lexAnalyzer->PeekCurrentToken();

			if (currentToken->GetLexem() == "[")
			{
				m_lexAnalyzer->GetNextToken();
				if (EofError("Error: Expected positive int"))
				{
					return;
				}
				currentToken = m_lexAnalyzer->PeekCurrentToken();

				if (currentToken->GetType() == TokenType::Int && std::atoi(&currentToken->GetLexem()[0]) >= 0)
				{
					data.length = std::atoi(&currentToken->GetLexem()[0]);

					m_lexAnalyzer->GetNextToken();
					if (EofError("Error: Expected ']'"))
					{
						return;
					}
					currentToken = m_lexAnalyzer->PeekCurrentToken();

					if (currentToken->GetLexem() != "]")
					{
						std::string token = "Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'";
						m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
						msclr::interop::marshal_as<String^>("Error: Expected ']'"), msclr::interop::marshal_as<String^>(token));

						ExitState();
						return;
					}

					m_lexAnalyzer->GetNextToken();
					if (EofError("Error: Expected ID or ','"))
					{
						return;
					}
					currentToken = m_lexAnalyzer->PeekCurrentToken();
				}
			}
			else
			{
				data.length = 0;

				if (EofError("Error: Expected '[', ':' or ','"))
				{
					return;
				}
			}
			m_names.push_back(data);

			if (currentToken->GetLexem() == ":")
			{
				break;
			}
			if (currentToken->GetLexem() == ",")
			{
				//We get a "," advance to the next token to look for other variable
				m_lexAnalyzer->GetNextToken();
				if (EofError("Error: ID"))
				{
					return;
				}
				currentToken = m_lexAnalyzer->PeekCurrentToken();
			}
			else
			{
				std::string token = "Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'";
				m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekPrevToken()->GetLine(),
				msclr::interop::marshal_as<String^>("Error: Expected '[', ':' or ','"), msclr::interop::marshal_as<String^>(token));

				ExitState();
				return;
			}
		}
		else
		{
			std::string token = "Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'";
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
			msclr::interop::marshal_as<String^>("Error: Expected ID"), msclr::interop::marshal_as<String^>(token));

			ExitState();
			return;
		}
	}

	if (EofError("Error: Expected ':'"))
	{
		return;
	}
	if (!isIDFound)
	{
		std::string token = "Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'";
		m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
		msclr::interop::marshal_as<String^>("Error: Expected ID"), msclr::interop::marshal_as<String^>(token));

		ExitState();
		return;
	}

	//Add All names to the Symbol Table
	m_lexAnalyzer->GetNextToken();
	if (EofError("Error: Expected DataType"))
	{
		return;
	}
	currentToken = m_lexAnalyzer->PeekCurrentToken();

	if (currentToken->GetLexem() == "int" || currentToken->GetLexem() == "float"
	|| currentToken->GetLexem() == "bool" || currentToken->GetLexem() == "string")
	{
		m_dataType = currentToken->GetLexem();

		m_lexAnalyzer->GetNextToken();
		if (EofError("Error: Expected ')'"))
		{
			return;
		}
		currentToken = m_lexAnalyzer->PeekCurrentToken();

		if (currentToken->GetLexem() != ")")
		{
			std::string token = "Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'";
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
			msclr::interop::marshal_as<String^>("Error: Expected ')'"), msclr::interop::marshal_as<String^>(token));

			ExitState();
			return;
		}

		for (int i = 0; i < m_names.size(); i++)
		{
			m_syntaxAnalyzer->GetSymbolTable()->AddSymbol(m_names[i].line, m_names[i].name, m_category, m_names[i].length, m_dataType, m_function);
		}

		ExitState();
	}
	else
	{
		std::string token = "Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'";
		m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
		msclr::interop::marshal_as<String^>("Error: Invalid Datatype"), msclr::interop::marshal_as<String^>(token));

		ExitState();
		return;
	}
}

void Compiler::SyntaxParameter::ExitState()
{
	Token* currentToken;

	if (EofError("Error: Expected ')'"))
	{
		return;
	}
	currentToken = m_lexAnalyzer->PeekCurrentToken();

	//Continue until the current expresion ends and go back to begin state
	while (!IsEof() && currentToken->GetLexem() != ")" && currentToken->GetLexem() != "var")
	{
		m_lexAnalyzer->GetNextToken();
	}
	if (!IsEof())
	{
		if (currentToken->GetLexem() == ")")
		{
			m_lexAnalyzer->GetNextToken();
		}
	}

	m_syntaxAnalyzer->EraseState();
}

bool Compiler::SyntaxParameter::EofError(std::string description)
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

bool Compiler::SyntaxParameter::IsEof()
{
	if (m_lexAnalyzer->GetTokenIteratior() == m_lexAnalyzer->GetNumTokens())
	{
		return true;
	}

	return false;
}

Compiler::SyntaxParameter::SyntaxParameter(LexAnalyzer* lexic, SyntaxAnalyzer* syntax, std::string function) : SyntaxState(lexic, syntax)
{
	m_function = function;
	m_category = SymbolCategory::Parameter;
}

Compiler::SyntaxParameter::~SyntaxParameter()
{
}
