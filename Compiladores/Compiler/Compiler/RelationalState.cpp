#include "pch.h"
#include "RelationalState.h"
#include "LexAnalyzer.h"
#include "BeginState.h"


void Compiler::RelationalState::Read(const char* source, LexAnalyzer& analyzer)
{
	unsigned long currentChar = analyzer.GetCurrentChar();

	if (source[currentChar] == '=')
	{
		if (source[currentChar - 1] == '=' || source[currentChar - 1] == '!' || source[currentChar - 1] == '>' || source[currentChar - 1] == '<')
		{
			m_buffer += source[currentChar];

			analyzer.IncCurrentChar();
			analyzer.AddToken(m_buffer, TokenType::Relational, analyzer.GetCurrentLine());
			analyzer.SetState(new BeginState());
		}
	}
	else
	{
		if (source[currentChar - 1] == '!')
		{
			analyzer.AddToken(m_buffer, TokenType::UnaryLogical, analyzer.GetCurrentLine());
			analyzer.SetState(new BeginState());
		}
		else if (source[currentChar - 1] == '=')
		{
			analyzer.AddToken(m_buffer, TokenType::Assign, analyzer.GetCurrentLine());
			analyzer.SetState(new BeginState());
		}
		else
		{
			analyzer.AddToken(m_buffer, TokenType::Relational, analyzer.GetCurrentLine());
			analyzer.SetState(new BeginState());
		}
	}
}

Compiler::RelationalState::RelationalState(const char* source, LexAnalyzer& analyzer)
{
	m_buffer += source[analyzer.GetCurrentChar()];
	analyzer.IncCurrentChar();
}

Compiler::RelationalState::~RelationalState()
{

}
