#include "Headers.h"
#include "SyntaxState.h"

namespace Compiler
{
	class LexAnalyzer;
	class SyntaxAnalyzer;

	class SyntaxFunction : public SyntaxState
	{
		bool							m_isReturning;

		SymbolCategory		m_category;
		std::string				m_returnValue;
		std::string				m_name;
		int								m_line;

	public:

		void							EofError(std::string description);
		void							CheckSyntax();
		void							PanicMode();
		
		SyntaxFunction(LexAnalyzer* lexic, SyntaxAnalyzer* syntax);
		~SyntaxFunction();
	};
}
