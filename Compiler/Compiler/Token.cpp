#include "pch.h"
#include "Token.h"

const unsigned long* Compiler::Token::GetLine()
{
	return &m_line;
}

const std::string* Compiler::Token::GetLexem()
{
	return &m_lexem;
}

const Compiler::TokenType* Compiler::Token::GetType()
{
	return &m_type;
}

Compiler::Token::Token(TokenType type, std::string lexem, unsigned long line)
{
	m_type = type;
	m_lexem = lexem;
	m_line = line;
}
