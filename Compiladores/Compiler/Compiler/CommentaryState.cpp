#include "pch.h"
#include "CommentaryState.h"
#include "LexAnalyzer.h"
#include "BeginState.h"

void Compiler::CommentaryState::Read(const char* source, LexAnalyzer& analyzer)
{
	unsigned long currentChar = analyzer.GetCurrentChar();

	if (source[currentChar] != '*')
	{
		m_buffer += source[currentChar];
		analyzer.IncCurrentChar();

		if (source[currentChar] == '\r')
		{
			analyzer.IncCurrentLine();
		}
		if (source[currentChar + 1] == '\0' || source[currentChar] == '\0')
		{
			//Error comentario no cerrado
			analyzer.AddError(ErrorPhase::Lexic, m_startLine, _COMMENT_NOT_CLOSED, 
			msclr::interop::marshal_as<String^>(m_buffer));
			analyzer.SetState(new BeginState());
		}
	}
	else
	{
		if (source[currentChar + 1] != '/')
		{
			analyzer.IncCurrentChar();
			m_buffer += source[currentChar];
		}
		else
		{
			analyzer.IncCurrentChar();
			analyzer.IncCurrentChar();
			analyzer.SetState(new BeginState());
		}
	}
}

Compiler::CommentaryState::CommentaryState(const char* source, LexAnalyzer& analyzer)
{
	if (source[analyzer.GetCurrentChar() - 1] == '/')
	{
		m_buffer += '/';
	}

	m_buffer += source[analyzer.GetCurrentChar()];
	analyzer.IncCurrentChar();
	m_startLine = analyzer.GetCurrentLine();
}


Compiler::CommentaryState::~CommentaryState()
{

}
