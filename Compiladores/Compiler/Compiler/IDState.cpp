#include "pch.h"
#include "Headers.h"
#include "LexAnalyzer.h"

#include "BeginState.h"
#include "IDState.h"

void Compiler::IDState::Read(const char* source, LexAnalyzer& analyzer)
{
	unsigned long currentChar = analyzer.GetCurrentChar();

	//Posibilities: ID, Keyword
	if (source[currentChar] == '_' || analyzer.IsAlpha(source[currentChar]) || analyzer.IsDigit(source[currentChar]))
	{
		m_buffer += source[currentChar];
		analyzer.IncCurrentChar();
	}
	else
	{
		if (source[currentChar - 1] != '_')
		{
			std::map<std::string, std::string> isKeyword = analyzer.GetKeyWordMap();

			if (isKeyword.find(m_buffer) != isKeyword.end())
			{
				analyzer.AddToken(m_buffer, Compiler::TokenType::Keyword, analyzer.GetCurrentLine());
			}
			else
			{
				analyzer.AddToken(m_buffer, Compiler::TokenType::ID, analyzer.GetCurrentLine());
			}
		}
		else
		{
			//Error, ID invalido
			analyzer.AddError(ErrorPhase::Lexic, analyzer.GetCurrentLine(), _LEX_INVALID_ID, msclr::interop::marshal_as<String^>(m_buffer));
		}
		analyzer.SetState(new BeginState());
	}
}

Compiler::IDState::IDState()
{
	m_buffer = "";
}


Compiler::IDState::~IDState()
{
}
