#pragma once
#include "Headers.h"

namespace Compiler
{
	class Token
	{
		TokenType m_type;
		std::string m_lexem;
		unsigned long m_line;

	public:

		const unsigned long* GetLine();
		const std::string* GetLexem();
		const TokenType* GetType();

		Token(TokenType type, std::string lexem, unsigned long line);
		Token() = default;
		~Token() = default;
	};
}
