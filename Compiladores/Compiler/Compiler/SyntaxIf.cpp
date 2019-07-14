#include "pch.h"
#include "SyntaxIf.h"
#include "SyntaxBlock.h"
#include "SyntaxLogicExpr.h"
#include "SyntaxAnalyzer.h"
#include "LexAnalyzer.h"

void Compiler::SyntaxIf::CheckSyntax() {
	Token* currentToken;

	if (!m_isReturningIf) {

		currentToken = m_lexAnalyzer->GetNextToken();

		if (!currentToken) {
			EofError("Error: Expected '('");
			return;
		}
		if (currentToken->GetLexem() == "(") {
			m_isReturningIf = true;
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
		if (!m_isReturningBlock) {
			currentToken = m_lexAnalyzer->PeekCurrentToken();

			if (currentToken->GetLexem() == "{") {
				m_isReturningBlock = true;
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
		else {
			currentToken = m_lexAnalyzer->PeekCurrentToken();
			if (!currentToken) {
				EofError("Error: Expected 'else'");
				return;
			}
			if (currentToken->GetLexem() == "else") {
				currentToken = m_lexAnalyzer->GetNextToken();
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
			else {
				m_syntaxAnalyzer->EraseState();
				return;
			}
		}
	}
}

void Compiler::SyntaxIf::EofError(std::string description) {
	m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekPrevToken()->GetLine(),
	msclr::interop::marshal_as<String^>(description), msclr::interop::marshal_as<String^>("Got-> 'eof'"));

	m_syntaxAnalyzer->EraseState();
}

void Compiler::SyntaxIf::PanicMode() {
	Token* currentToken = m_lexAnalyzer->PeekCurrentToken();

	if (!m_isReturningBlock) {
		while (currentToken && currentToken->GetLexem() != "}" && currentToken->GetLexem() != "else") {
			currentToken = m_lexAnalyzer->GetNextToken();
		}
		if (!currentToken) {
			EofError("Error: Unexpected token in 'if' in " + m_scope + "'");
			return;
		}
		if (currentToken->GetLexem() == "else") {
			return;
		}
		else {
			m_syntaxAnalyzer->EraseState();
			return;
		}
	}
	else {
		while (currentToken && currentToken->GetLexem() != "}") {
			currentToken = m_lexAnalyzer->GetNextToken();
		}
		if (!currentToken) {
			EofError("Error: Unexpected token in 'else' in " + m_scope + "'");
			return;
		}
		if (currentToken->GetLexem() == "else") {
			return;
		}
		else {
			m_syntaxAnalyzer->EraseState();
			return;
		}
	}
}

Compiler::SyntaxIf::SyntaxIf(LexAnalyzer* lexic, SyntaxAnalyzer* syntax, std::string scope) 
: SyntaxState(lexic, syntax) {
	m_isReturningBlock = false;
	m_isReturningIf = false;
	m_scope = scope;
}

Compiler::SyntaxIf::~SyntaxIf() {

}
