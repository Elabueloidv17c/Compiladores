#pragma once
#include "Headers.h"
#include "GlobalNode.h"
#include "ErrorModule.h"

namespace Compiler
{
	class SymbolTable
	{
	private:

		std::map<std::string, GlobalNode*>			m_symbols;
		msclr::gcroot<ErrorModule^>							m_errorModule;

	public:

		std::vector <std::string> GetStrSymbols();
		void AddSymbol(int line, std::string name, SymbolCategory category, int dimension, std::string dataType, std::string scope);
		bool SymbolExists(std::string symbol, SymbolCategory category, std::string function);
		void Clear();

		SymbolTable(ErrorModule^ errors);
		~SymbolTable();
	};
}