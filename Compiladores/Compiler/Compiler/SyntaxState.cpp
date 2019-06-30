#include "pch.h"
#include "SyntaxState.h"

Compiler::SyntaxState::SyntaxState(LexAnalyzer* lexic, SyntaxAnalyzer* syntax)
{
	m_lexAnalyzer = lexic;
	m_syntaxAnalyzer = syntax;
}

Compiler::SyntaxState::~SyntaxState()
{
}

void Compiler::SyntaxState::CheckSyntax()
{

}