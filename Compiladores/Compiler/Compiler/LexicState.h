#pragma once

namespace Compiler
{
	class LexAnalyzer;

	class LexicState
	{
	public:

		virtual void Read(const char* source, LexAnalyzer& analyzer);

		LexicState();
		virtual ~LexicState();
	};
}