#pragma once
#include "windows.h"
#include "vcclr.h"
#include "Headers.h"
#include "ErrorModule.h"
#include "Token.h"
#include "LexicState.h"

namespace Compiler
{
	class LexAnalyzer
	{
		msclr::gcroot<ErrorModule^>					m_errorModule;
		std::vector<Token>*							m_tokens;
		int											m_currentToken;

		std::map<std::string, std::string>			m_keywords;
		
		unsigned long								m_currentChar;
		unsigned long								m_currentLine;
		LexicState*									m_state;

	public:

		LexAnalyzer(ErrorModule^ errorModule);
		LexAnalyzer();
		~LexAnalyzer();

		bool										ParseSourceCode(const char* source);
		void										SetState(LexicState* state);
		void										Read(const char* source);

		bool										IsAlpha(const char value);
		bool										IsDigit(const char value);

		bool										AddError(ErrorPhase phase, int lineNumber, String^ errorDescription, String^ errorLine);
		void										AddToken(std::string lexem, TokenType type, unsigned int line);

		msclr::gcroot<ErrorModule^>					GetErrorModule();

		void										SetCurrentChar(unsigned long index);
		void										SetCurrentLine(unsigned long index);
		void										SetCurrentToken(int index);
		unsigned long								GetCurrentChar();
		void										IncCurrentChar();
		void										DecCurrentChar();

		unsigned long								GetCurrentLine();
		void										IncCurrentLine();

		std::map<std::string, std::string>			GetKeyWordMap();
		void										ClearTokens();

		void										SetTokenIterator(int index);
		int											GetTokenIteratior();
		int											GetNumTokens();
		std::vector <Token>*						GetTokens();

		Token*										PeekTokenAt(int index);
		Token*										PeekCurrentToken();
		Token*										PeekNextToken();
		Token*										PeekPrevToken();

		Token*										GetTokenAt(int index);
		Token*										GetNextToken();
		Token*										GetPrevToken();
	};
}