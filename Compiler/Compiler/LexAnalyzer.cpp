#pragma once
#include "pch.h"
#include "LexAnalyzer.h"
#include "windows.h"
#include "vcclr.h"
#include "BeginState.h"

bool Compiler::LexAnalyzer::ParseSourceCode(const char* source)
{
	while (source[m_currentChar] != '\0' && m_errorModule->GetErrorNumber() < _MAX_ERRORS)
	{
		Read(source);
	}
	if (m_errorModule->GetErrorNumber() < _MAX_ERRORS)
	{
		Read(source);
	}

	return true;
}

void Compiler::LexAnalyzer::SetCurrentChar(unsigned long index)
{
	m_currentChar = index;
}

void Compiler::LexAnalyzer::SetCurrentLine(unsigned long index)
{
	m_currentLine = index;
}

void Compiler::LexAnalyzer::IncCurrentChar()
{
	m_currentChar++;
}

void Compiler::LexAnalyzer::DecCurrentChar()
{
	m_currentChar--;
}

unsigned long Compiler::LexAnalyzer::GetCurrentChar()
{
	return m_currentChar;
}

std::map<std::string, std::string> Compiler::LexAnalyzer::GetKeyWordMap()
{
	return m_keywords;
}

unsigned long Compiler::LexAnalyzer::GetCurrentLine()
{
	return m_currentLine;
}

void Compiler::LexAnalyzer::IncCurrentLine()
{
	m_currentLine++;
}

Compiler::LexAnalyzer::LexAnalyzer(ErrorModule^ errorModule)
{
	m_errorModule = errorModule;

	m_currentChar = 0;
	m_currentLine = 1;

	m_state = new BeginState();
	m_tokens = new std::vector<Token>();

	m_keywords.insert(std::make_pair("var", ""));
	m_keywords.insert(std::make_pair("void", ""));
	m_keywords.insert(std::make_pair("bool", ""));
	m_keywords.insert(std::make_pair("int", ""));
	m_keywords.insert(std::make_pair("float", ""));
	m_keywords.insert(std::make_pair("string", ""));

	m_keywords.insert(std::make_pair("true", ""));
	m_keywords.insert(std::make_pair("false", ""));

	m_keywords.insert(std::make_pair("print", ""));
	m_keywords.insert(std::make_pair("read", ""));

	m_keywords.insert(std::make_pair("main", ""));

	m_keywords.insert(std::make_pair("if", ""));
	m_keywords.insert(std::make_pair("else", ""));

	m_keywords.insert(std::make_pair("while", ""));
	m_keywords.insert(std::make_pair("for", ""));

	m_keywords.insert(std::make_pair("switch", ""));
	m_keywords.insert(std::make_pair("case", ""));
	m_keywords.insert(std::make_pair("default", ""));

	m_keywords.insert(std::make_pair("function", ""));
	m_keywords.insert(std::make_pair("return", ""));

	m_keywords.insert(std::make_pair("inc", ""));
	m_keywords.insert(std::make_pair("dec", ""));
}

void Compiler::LexAnalyzer::SetState(LexicState* state)
{
	if (m_state)
	{
		delete m_state;
		m_state = nullptr;
	}

	m_state = state;
}

void Compiler::LexAnalyzer::Read(const char* source)
{
	m_state->Read(source, *this);
}

bool Compiler::LexAnalyzer::AddError(ErrorPhase phase, int lineNumber, String ^ errorDescription, String ^ errorLine)
{
	return m_errorModule->AddError(phase, lineNumber, errorDescription, errorLine);
}

void Compiler::LexAnalyzer::AddToken(std::string lexem, TokenType type, unsigned int line)
{
	m_tokens->push_back(Token(line, type, lexem));
}

std::vector<Compiler::Token>* Compiler::LexAnalyzer::GetTokens()
{
	return m_tokens;
}

msclr::gcroot<Compiler::ErrorModule^> Compiler::LexAnalyzer::GetErrorModule()
{
	return m_errorModule;
}

void Compiler::LexAnalyzer::ClearTokens()
{
	m_tokens->clear();
}

Compiler::Token* Compiler::LexAnalyzer::GetCurrentToken()
{
	return &m_tokens[0][m_tokens->size() - 1];
}

Compiler::LexAnalyzer::LexAnalyzer()
{
	m_currentChar = 0;
	m_currentChar = 1;

	m_state = new BeginState();
	m_tokens = new std::vector<Token>();
}

Compiler::LexAnalyzer::~LexAnalyzer()
{
	if (m_state)
	{
		delete m_state;
		m_state = nullptr;
	}

	m_keywords.clear();

	if (m_tokens)
	{
		ClearTokens();
		delete m_tokens;
		m_tokens = nullptr;
	}

}

bool Compiler::LexAnalyzer::IsAlpha(const char value)
{
	char lowerChar = tolower(value);

	if (lowerChar > 96 && lowerChar < 123)
	{
		return true;
	}

	return false;
}

bool Compiler::LexAnalyzer::IsDigit(const char value)
{
	if (value > 47 && value < 58)
	{
		return true;
	}

	return false;
}