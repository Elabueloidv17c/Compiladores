#include "Headers.h"
#include "SyntaxState.h"

namespace Compiler
{
	class LexAnalyzer;
	class SyntaxAnalyzer;

	class SyntaxFunction : public SyntaxState
	{
		bool							m_isParameterReached;
		bool							m_isBlockReached;
		std::string						m_eofMessage;

		SymbolCategory					m_category;
		std::string						m_returnValue;
		std::string						m_name;
		int								m_line;

	public:

		bool							EofError(std::string description);
		bool							IsEof();

		void							CheckSyntax();
		void							ExitState();
		
		SyntaxFunction(LexAnalyzer* lexic, SyntaxAnalyzer* syntax);
		~SyntaxFunction();
	};
}
