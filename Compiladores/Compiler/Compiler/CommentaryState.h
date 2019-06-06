#pragma once
#include "Headers.h"
#include "LexicState.h"

namespace Compiler
{
	class LexAnalyzer;

	class CommentaryState : public LexicState
	{
		unsigned long			m_startLine;
		std::string				m_buffer;

	public:

		void Read(const char* source, LexAnalyzer& analyzer);

		CommentaryState(const char* source, LexAnalyzer& analyzer);
		~CommentaryState();
	};
}