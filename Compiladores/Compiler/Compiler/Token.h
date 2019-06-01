#pragma once
#include "Headers.h"

namespace Compiler
{
	class Token
	{
		TokenType				m_type;
		std::string				m_lexem;
		unsigned int			m_line;

	public:

		unsigned long		GetLine();
		std::string			GetLexem();
		TokenType			GetType();
		std::string			GetTypeStr();

		Token(unsigned int line, TokenType type, std::string lexem);
		Token() = default;
		~Token() = default;
	};
}
