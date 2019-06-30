#include "pch.h"
#include "Token.h"

unsigned long Compiler::Token::GetLine()
{
	return m_line;
}

std::string Compiler::Token::GetLexem()
{
	return m_lexem;
}

Compiler::TokenType Compiler::Token::GetType()
{
	return m_type;
}

std::string Compiler::Token::GetTypeStr()
{
	if (m_type == TokenType::ID)
	{
		return "ID";
	}
	if (m_type == TokenType::Int)
	{
		return "Int";
	}
	if (m_type == TokenType::Float)
	{
		return "Float";
	}
	if (m_type == TokenType::String)
	{
		return "String";
	}
	if (m_type == TokenType::Relational)
	{
		return "Relational";
	}
	if (m_type == TokenType::Arithmetic)
	{
		return "Arithmetic";
	}
	if (m_type == TokenType::Logical)
	{
		return "Logical";
	}
	if (m_type == TokenType::UnaryLogical)
	{
		return "Unary Logical";
	}
	if (m_type == TokenType::Negation)
	{
		return "Negation";
	}
	if (m_type == TokenType::Separator)
	{
		return "Separator";
	}
	if (m_type == TokenType::Assign)
	{
		return "Assign";
	}
	if (m_type == TokenType::Dimension)
	{
		return "Dimension";
	}
	if (m_type == TokenType::Grouping)
	{
		return "Grouping";
	}
	if (m_type == TokenType::Keyword)
	{
		return "Keyword";
	}
}

Compiler::Token::Token(unsigned int line, TokenType type, std::string lexem)
{
	m_type = type;
	m_lexem = lexem;
	m_line = line;
}
