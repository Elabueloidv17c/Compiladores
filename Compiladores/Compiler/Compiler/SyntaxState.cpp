#include "pch.h"
#include "SyntaxState.h"

Compiler::SyntaxState::SyntaxState(LexAnalyzer* lexic, SyntaxAnalyzer* syntax)
{
	m_lexAnalyzer = lexic;
	m_syntaxAnalyzer = syntax;
}

Compiler::SyntaxState::~SyntaxState() {

}

void Compiler::SyntaxState::CheckSyntax() {

}

void Compiler::SyntaxState::EofError(std::string description) {

}

void Compiler::SyntaxState::PanicMode() {

}
