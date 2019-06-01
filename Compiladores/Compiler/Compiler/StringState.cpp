#include "pch.h"
#include "StringState.h"
#include "LexAnalyzer.h"
#include "BeginState.h"

void Compiler::StringState::Read(const char* source, LexAnalyzer& analyzer)
{
	unsigned long currentChar = analyzer.GetCurrentChar();

	if (source[currentChar] != '"')
	{
		m_buffer += source[currentChar];
		analyzer.IncCurrentChar();

		if (source[currentChar] == '\r' || source[currentChar + 1] == '\0')
		{
			analyzer.AddError(ErrorPhase::Lexic, analyzer.GetCurrentLine(), _STRING_NOT_CLOSED, 
			msclr::interop::marshal_as<String^>(m_buffer));
			analyzer.SetState(new BeginState());
			analyzer.IncCurrentLine();
			//Error string no cerrado (seguir leyendo)
		}
	}
	else
	{
		m_buffer += source[currentChar];
		analyzer.IncCurrentChar();

		analyzer.AddToken(m_buffer, TokenType::String, analyzer.GetCurrentLine());
		analyzer.SetState(new BeginState());
	}
}

Compiler::StringState::StringState(const char* source, LexAnalyzer& analyzer)
{
	m_buffer += source[analyzer.GetCurrentChar()];
	analyzer.IncCurrentChar();
}

Compiler::StringState::~StringState()
{
}
