#include "pch.h"
#include "LexAnalyzer.h"
#include "BeginState.h"
#include "FloatState.h"


void Compiler::FloatState::Read(const char* source, LexAnalyzer& analyzer)
{
	unsigned long currentChar = analyzer.GetCurrentChar();

	if (analyzer.IsDigit(source[currentChar]))
	{
		m_buffer += source[currentChar];
		analyzer.IncCurrentChar();
	}
	else if (source[currentChar] == '.' && source[currentChar - 1] == '.')
	{
		if (analyzer.GetTokens()->size())
		{
			m_buffer = analyzer.PeekCurrentToken()->GetLexem() + source[currentChar - 1];
		}
		analyzer.AddError(ErrorPhase::Lexic, analyzer.GetCurrentLine(), _INVALID_FLOAT,
		msclr::interop::marshal_as<String^>(m_buffer));

		m_buffer = ".";
		analyzer.IncCurrentChar();
	}
	else
	{
		if (((source[currentChar] != '\0' || analyzer.IsDigit(source[currentChar - 1]))) && source[currentChar - 1] != '.')
		{
			analyzer.AddToken(m_buffer, Compiler::TokenType::Float, analyzer.GetCurrentLine());
		}
		else if(source[currentChar - 1] != '.')
		{
			analyzer.AddError(ErrorPhase::Lexic, analyzer.GetCurrentLine(), _INVALID_FLOAT,
			msclr::interop::marshal_as<String^>(m_buffer));
		}

		analyzer.SetState(new BeginState());
	}
}

Compiler::FloatState::FloatState(std::string intPart)
{
	m_buffer = intPart;
}

Compiler::FloatState::FloatState(const char* source, LexAnalyzer& analyzer)
{
	if (analyzer.GetCurrentChar() > 0)
	{
		if (source[analyzer.GetCurrentChar() - 1] == '-')
		{
			m_buffer += '-';
		}
	}

	m_buffer += source[analyzer.GetCurrentChar()];
	analyzer.IncCurrentChar();
}

Compiler::FloatState::~FloatState()
{

}
