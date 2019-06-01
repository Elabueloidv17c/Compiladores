#include "pch.h"
#include "LogicalState.h"
#include "LexAnalyzer.h"
#include "BeginState.h"


void Compiler::LogicalState::Read(const char* source, LexAnalyzer& analyzer)
{
	unsigned long currentChar = analyzer.GetCurrentChar();

	if (source[currentChar - 1] == source[currentChar])
	{
		m_buffer += source[currentChar];

		analyzer.IncCurrentChar();
		analyzer.AddToken(m_buffer, TokenType::Logical, analyzer.GetCurrentLine());
		analyzer.SetState(new BeginState());
	}
	else
	{
		analyzer.AddError(ErrorPhase::Lexic, analyzer.GetCurrentLine(), _INVALID_OP_LOG, msclr::interop::marshal_as<String^>(m_buffer));
		analyzer.SetState(new BeginState());
	}
}

Compiler::LogicalState::LogicalState(const char* source, LexAnalyzer& analyzer)
{
	m_buffer += source[analyzer.GetCurrentChar()];
	analyzer.IncCurrentChar();
}


Compiler::LogicalState::~LogicalState()
{
}
