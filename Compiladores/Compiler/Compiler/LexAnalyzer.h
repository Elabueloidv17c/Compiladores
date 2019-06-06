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

		std::map<std::string, std::string>			m_keywords;
		
		unsigned long								m_currentChar;
		unsigned long								m_currentLine;
		LexicState*									m_state;

	public:

		LexAnalyzer(ErrorModule^ errorModule);
		LexAnalyzer();
		~LexAnalyzer();

		bool										IsAlpha(const char value);
		bool										IsDigit(const char value);

		bool										AddError(ErrorPhase phase, int lineNumber, String^ errorDescription, String^ errorLine);
		void										AddToken(std::string lexem, TokenType type, unsigned int line);
		bool										ParseSourceCode(const char* source);
		void										SetState(LexicState* state);
		void										Read(const char* source);
		std::vector <Token>*						GetTokens();

		msclr::gcroot<ErrorModule^>					GetErrorModule();

		void										SetCurrentChar(unsigned long index);
		void										SetCurrentLine(unsigned long index);
		unsigned long								GetCurrentChar();
		void										IncCurrentChar();
		void										DecCurrentChar();

		unsigned long								GetCurrentLine();
		void										IncCurrentLine();

		std::map<std::string, std::string>			GetKeyWordMap();
		void										ClearTokens();

		Token*										GetCurrentToken();
		//Token*										GetTokenAt(int index);
		//Token*										GetNextToken();
		//Token*										GetPrevToken();
	};
}