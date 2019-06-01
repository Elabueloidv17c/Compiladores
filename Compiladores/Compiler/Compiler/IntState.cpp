#include "pch.h"
#include "LexAnalyzer.h"
#include "BeginState.h"
#include "IntState.h"
#include "FloatState.h"

void Compiler::IntState::Read(const char* source, LexAnalyzer& analyzer)
{
	unsigned long currentChar = analyzer.GetCurrentChar();

	if (analyzer.IsDigit(source[currentChar]))
	{
		m_buffer += source[currentChar];
		analyzer.IncCurrentChar();
	}
	else if (source[currentChar] == '.')
	{
		m_buffer += source[currentChar];
		analyzer.IncCurrentChar();
		analyzer.SetState(new FloatState(m_buffer));
	}
	else
	{
		analyzer.AddToken(m_buffer, Compiler::TokenType::Int, analyzer.GetCurrentLine());
		analyzer.SetState(new BeginState());
	}
}

Compiler::IntState::IntState(bool isNegative)
{
	if (isNegative)
	{
		m_buffer = "-";
	}
}


Compiler::IntState::~IntState()
{
}
