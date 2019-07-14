#include "pch.h"
#include "SyntaxPrint.h"
#include "SyntaxLogicExprList.h"
#include "SyntaxAnalyzer.h"
#include "LexAnalyzer.h"

void Compiler::SyntaxPrint::CheckSyntax() {
	Token* currentToken;

	if (!m_isReturning) {
		currentToken = m_lexAnalyzer->GetNextToken();

		if (!currentToken) {
			EofError("Error: Expected '('");
			return;
		}
		if (currentToken->GetLexem() == "(") {
			currentToken = m_lexAnalyzer->GetNextToken();

			if (!currentToken) {
				EofError("Error: Expected Logic Expression, ID, Constant or ')' in Function call in '" + m_scope + "'");
				return;
			}
			if (currentToken->GetLexem() == ")") {
				m_isReturning = true;
				return;
			}
			if (currentToken->GetLexem() == "(" || currentToken->GetLexem() == "true" || currentToken->GetLexem() == "false"
				|| currentToken->GetType() == TokenType::Int || currentToken->GetType() == TokenType::String
				|| currentToken->GetType() == TokenType::ID || currentToken->GetType() == TokenType::Float
				|| currentToken->GetType() == TokenType::UnaryLogical)
			{
				m_isReturning = true;
				m_syntaxAnalyzer->AddState(new SyntaxLogicExprList(m_lexAnalyzer, m_syntaxAnalyzer, m_scope));
				return;
			}
			else {
				m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
					msclr::interop::marshal_as<String^>("Error: Expected Logic expression list in Function call in '" + m_scope + "'"),
					msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));

				PanicMode();
				return;
			}
		}
		else {
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
				msclr::interop::marshal_as<String^>("Error: Expected '(' in Function call in '" + m_scope + "'"),
				msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));

			PanicMode();
			return;
		}
	}
	else {
		currentToken = m_lexAnalyzer->PeekCurrentToken();

		if (!currentToken) {
			EofError("Error: Expected ')'");
			return;
		}
		if (currentToken->GetLexem() == ")") {
			currentToken = m_lexAnalyzer->GetNextToken();

			if (!currentToken) {
				EofError("Error: Expected ';' in Function call in'" + m_scope + "'");
				return;
			}
			if (currentToken->GetLexem() == ";") {
				currentToken = m_lexAnalyzer->GetNextToken();
				m_syntaxAnalyzer->EraseState();
				return;
			}
			else {
				m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
					msclr::interop::marshal_as<String^>("Error: Expected ';' in Function call in '" + m_scope + "'"),
					msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));

				PanicMode();
				return;
			}
		}
		else {
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
				msclr::interop::marshal_as<String^>("Error: Expected ')' in Function call in '" + m_scope + "'"),
				msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));

			PanicMode();
			return;
		}
	}
}

void Compiler::SyntaxPrint::EofError(std::string description) {
	m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekPrevToken()->GetLine(),
		msclr::interop::marshal_as<String^>(description), msclr::interop::marshal_as<String^>("Got-> 'eof'"));

	m_syntaxAnalyzer->EraseState();
}

void Compiler::SyntaxPrint::PanicMode() {
	Token* currentToken = m_lexAnalyzer->PeekCurrentToken();

	if (!currentToken) {
		EofError("Error: Unexpected Token in Print in '" + m_scope + "'");
		return;
	}
	while (currentToken && currentToken->GetLexem() != ";" && currentToken->GetLexem() != "}") {
		currentToken = m_lexAnalyzer->GetNextToken();
	}
	if (!currentToken) {
		EofError("Error: Unexpected Token in Print in '" + m_scope + "'");
		return;
	}
	if (currentToken->GetLexem() == ";") {
		m_lexAnalyzer->GetNextToken();
	}
	m_syntaxAnalyzer->EraseState();
}

Compiler::SyntaxPrint::SyntaxPrint(LexAnalyzer* lexic, SyntaxAnalyzer* syntax, std::string scope)
	: SyntaxState(lexic, syntax) {
	m_isReturning = false;
	m_scope = scope;
}

Compiler::SyntaxPrint::~SyntaxPrint() {

}
