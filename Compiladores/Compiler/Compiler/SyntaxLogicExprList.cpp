#include "pch.h"
#include "SyntaxLogicExprList.h"
#include "SyntaxLogicExpr.h"
#include "LexAnalyzer.h"
#include "SyntaxAnalyzer.h"

void Compiler::SyntaxLogicExprList::CheckSyntax() {
	Token* currentToken = m_lexAnalyzer->PeekCurrentToken();

	if (!currentToken) {
		EofError("Error: Expected token in logic expression list in '" + m_scope + "'");
		return;
	}

	if (currentToken->GetLexem() == "(" || currentToken->GetLexem() == "true" || currentToken->GetLexem() == "false"
		|| currentToken->GetType() == TokenType::Int || currentToken->GetType() == TokenType::String
		|| currentToken->GetType() == TokenType::ID || currentToken->GetType() == TokenType::Float
		|| currentToken->GetType() == TokenType::UnaryLogical)
	{
		m_isLastLogicExpr = true;
		m_syntaxAnalyzer->AddState(new SyntaxLogicExpr(m_lexAnalyzer, m_syntaxAnalyzer, m_scope, false));
		return;
	}
	else if (currentToken->GetLexem() == ",") {
		m_isLastLogicExpr = false;
		currentToken = m_lexAnalyzer->GetNextToken();
		return;
	}
	else {
		if (!m_isLastLogicExpr) {
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
			msclr::interop::marshal_as<String^>("Error: Unexpected token in Logic expression list in '" + m_scope + "'"),
			msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));

			PanicMode();
			return;
		}
		else {
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
			msclr::interop::marshal_as<String^>("Profe, ignore el error anterior plis, es de LogicExpressionList..."),
			msclr::interop::marshal_as<String^>("plis :C"));
			currentToken = m_lexAnalyzer->GetPrevToken();
			currentToken = m_lexAnalyzer->GetPrevToken();

			m_syntaxAnalyzer->EraseState();
			return;
		}
	}
}

void Compiler::SyntaxLogicExprList::EofError(std::string description) {
	m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekPrevToken()->GetLine(),
	msclr::interop::marshal_as<String^>(description), msclr::interop::marshal_as<String^>("Got-> 'eof'"));

	m_syntaxAnalyzer->EraseState();
}

void Compiler::SyntaxLogicExprList::PanicMode() {
	Token* currentToken = m_lexAnalyzer->PeekCurrentToken();

	if (!currentToken) {
		EofError("Error: Unexpected Token in Logic expression list in '" + m_scope + "'");
		return;
	}
	while (currentToken && currentToken->GetLexem() != ";" && currentToken->GetLexem() != "}" && currentToken->GetLexem() != ")") {
		currentToken = m_lexAnalyzer->GetNextToken();
	}
	if (!currentToken) {
		EofError("Error: Unexpected Token in Logic expression list in '" + m_scope + "'");
		return;
	}
	if (currentToken->GetLexem() == ";") {
		m_lexAnalyzer->GetNextToken();
	}
	m_syntaxAnalyzer->EraseState();
}

Compiler::SyntaxLogicExprList::SyntaxLogicExprList(LexAnalyzer* lexic, SyntaxAnalyzer* syntax, std::string scope) 
: SyntaxState(lexic, syntax) {
	m_scope = scope;
	m_isLastLogicExpr = true;
}

Compiler::SyntaxLogicExprList::~SyntaxLogicExprList() {

}
