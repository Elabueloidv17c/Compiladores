#include "pch.h"
#include "ArithmeticState.h"
#include "CommentaryState.h"
#include "LexAnalyzer.h"
#include "FloatState.h"
#include "BeginState.h"
#include "IntState.h"


void Compiler::ArithmeticState::Read(const char* source, LexAnalyzer& analyzer)
{
	unsigned long currentChar = analyzer.GetCurrentChar();

	if (analyzer.IsDigit(source[currentChar]) && source[currentChar - 1] == '-')
	{
		analyzer.SetState(new IntState(true));
		return;
	}
	if (source[currentChar] == '.' && source[currentChar - 1] == '-')
	{
		analyzer.SetState(new FloatState(source, analyzer));
		return;
	}
	if (source[currentChar] == '*' && source[currentChar - 1] == '/')
	{
		analyzer.SetState(new CommentaryState(source, analyzer));
	}
	else
	{
		analyzer.AddToken(m_buffer, Compiler::TokenType::Arithmetic, analyzer.GetCurrentLine());
		analyzer.SetState(new BeginState());
	}
}

Compiler::ArithmeticState::ArithmeticState(const char* source, LexAnalyzer& analyzer)
{
	m_buffer += source[analyzer.GetCurrentChar()];
	analyzer.IncCurrentChar();
}


Compiler::ArithmeticState::~ArithmeticState()
{
}
