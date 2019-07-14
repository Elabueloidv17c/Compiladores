#include "pch.h"
#include "SyntaxWhile.h"
#include "SyntaxBlock.h"
#include "SyntaxLogicExpr.h"
#include "SyntaxAnalyzer.h"
#include "LexAnalyzer.h"

void Compiler::SyntaxWhile::CheckSyntax() {
	Token* currentToken;

	if (!m_isReturning) {

		currentToken = m_lexAnalyzer->GetNextToken();

		if (!currentToken) {
			EofError("Error: Expected '('");
			return;
		}
		if (currentToken->GetLexem() == "(") {
			m_isReturning = true;
			m_syntaxAnalyzer->AddState(new SyntaxLogicExpr(m_lexAnalyzer, m_syntaxAnalyzer, m_scope, true));
			return;
		}
		else {
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
			"Error: Expected '(' ",
			msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));

			PanicMode();
			return;
		}
	}
	else {
		currentToken = m_lexAnalyzer->PeekCurrentToken();

		if (!currentToken) {
			EofError("Error: Expected '{'");
			return;
		}
		if (currentToken->GetLexem() == "{") {
			m_syntaxAnalyzer->EraseState();
			m_syntaxAnalyzer->AddState(new SyntaxBlock(m_lexAnalyzer, m_syntaxAnalyzer, m_scope));
			return;
		}
		else {
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
			"Error: Expected '{'",
			msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));

			PanicMode();
			return;
		}
	}
}

void Compiler::SyntaxWhile::EofError(std::string description) {
	m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekPrevToken()->GetLine(),
	msclr::interop::marshal_as<String^>(description), msclr::interop::marshal_as<String^>("Got-> 'eof'"));

	m_syntaxAnalyzer->EraseState();
}

void Compiler::SyntaxWhile::PanicMode() {
	Token* currentToken = m_lexAnalyzer->PeekCurrentToken();

	while (currentToken && currentToken->GetLexem() != "}") {
		currentToken = m_lexAnalyzer->GetNextToken();
	}
	if (!currentToken) {
		EofError("Error: Unexpected token in While" + m_scope + "'");
		return;
	}
	else {
		m_syntaxAnalyzer->EraseState();
		return;
	}
}

Compiler::SyntaxWhile::SyntaxWhile(LexAnalyzer* lexic, SyntaxAnalyzer* syntax, std::string scope)
: SyntaxState(lexic, syntax) {
	m_isReturning = false;
	m_scope = scope;
}

Compiler::SyntaxWhile::~SyntaxWhile() {

}
