#include "pch.h"
#include "SyntaxAnalyzer.h"
#include "SyntaxState.h"
#include "SyntaxBegin.h"

Compiler::SymbolTable* Compiler::SyntaxAnalyzer::GetSymbolTable()
{
	return m_symbolTable;
}

void Compiler::SyntaxAnalyzer::AddState(SyntaxState* state)
{
	m_states.push(state);
}

void Compiler::SyntaxAnalyzer::EraseState()
{
	m_states.pop();
}

void Compiler::SyntaxAnalyzer::ClearStates()
{
	while (!m_states.empty())
	{
		m_states.pop();
	}
}

void Compiler::SyntaxAnalyzer::SetMainAdded(bool state)
{
	m_isMainAdded = state;
}

bool Compiler::SyntaxAnalyzer::GetMainStatus()
{
	return m_isMainAdded;
}

void Compiler::SyntaxAnalyzer::CheckSyntax()
{
	m_states.push(new SyntaxBegin(m_lexic, this));

	while (m_lexic->GetTokenIteratior() < m_lexic->GetNumTokens() && m_errorModule->GetErrorNumber() < _MAX_ERRORS) {
		m_states.top()->CheckSyntax();
	}
	if (m_errorModule->GetErrorNumber() < _MAX_ERRORS) {
		m_states.top()->CheckSyntax();
	}
}

Compiler::SyntaxAnalyzer::SyntaxAnalyzer(LexAnalyzer* lexic, ErrorModule^ errors, SymbolTable* symbols)
{
	m_isMainAdded = false;
	m_errorModule = errors;
	m_symbolTable = symbols;
	m_lexic = lexic;
	m_states.push(new SyntaxBegin(m_lexic, this));
}

Compiler::SyntaxAnalyzer::~SyntaxAnalyzer()
{

}

