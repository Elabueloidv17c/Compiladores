#include "pch.h"
#include "SyntaxRead.h"
#include "SyntaxLogicExpr.h"
#include "SyntaxAnalyzer.h"
#include "LexAnalyzer.h"

void Compiler::SyntaxRead::CheckSyntax() {
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
				EofError("Error: Expected ID");
				return;
			}
			if (currentToken->GetType() == TokenType::ID) {
				m_isReturning = true;
				currentToken = m_lexAnalyzer->GetNextToken();

				if (!currentToken) {
					EofError("Error: Expected '[' or ')' in Read in '" + m_scope + "'");
					return;
				}
				if (currentToken->GetLexem() == "[") {
					m_isDimension = true;
					currentToken = m_lexAnalyzer->GetNextToken();

					if (!currentToken) {
						EofError("Error: Expected Logic Expression, ID or Constant in Read in '" + m_scope + "'");
						return;
					}
					if (currentToken->GetLexem() == "(" || currentToken->GetLexem() == "true" || currentToken->GetLexem() == "false"
						|| currentToken->GetType() == TokenType::Int || currentToken->GetType() == TokenType::String
						|| currentToken->GetType() == TokenType::ID || currentToken->GetType() == TokenType::Float
						|| currentToken->GetType() == TokenType::UnaryLogical)
					{
						m_syntaxAnalyzer->AddState(new SyntaxLogicExpr(m_lexAnalyzer, m_syntaxAnalyzer, m_scope, false));
						return;
					}
					else {
						m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
						msclr::interop::marshal_as<String^>("Error: Expected Logic expression in Read in '" + m_scope + "'"),
						msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));

						PanicMode();
						return;
					}
				}
			}
			else {
				m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
				msclr::interop::marshal_as<String^>("Error: Expected ID or constant in Read in '" + m_scope + "'"),
				msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));

				PanicMode();
				return;
			}
		}
		else {
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
			msclr::interop::marshal_as<String^>("Error: Expected '(' in Read in '" + m_scope + "'"),
			msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));

			PanicMode();
			return;
		}
	}
	else {
		currentToken = m_lexAnalyzer->PeekCurrentToken();

		if (m_isDimension) {
			if (!currentToken) {
				EofError("Error: Expected ']'");
				return;
			}
			if (currentToken->GetLexem() == "]") {
				currentToken = m_lexAnalyzer->GetNextToken();
			}
			else {
				m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
				msclr::interop::marshal_as<String^>("Error: Expected ']' in Read in '" + m_scope + "'"),
				msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));

				PanicMode();
				return;
			}
		}

		if (!currentToken) {
			EofError("Error: Expected ')'");
			return;
		}
		if (currentToken->GetLexem() == ")") {
			currentToken = m_lexAnalyzer->GetNextToken();

			if (!currentToken) {
				EofError("Error: Expected ';' in Read in'" + m_scope + "'");
				return;
			}
			if (currentToken->GetLexem() == ";") {
				currentToken = m_lexAnalyzer->GetNextToken();
				m_syntaxAnalyzer->EraseState();
				return;
			}
			else {
				m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
				msclr::interop::marshal_as<String^>("Error: Expected ';' in Read in '" + m_scope + "'"),
				msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));

				PanicMode();
				return;
			}
		}
		else {
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
			msclr::interop::marshal_as<String^>("Error: Expected ')' in Read in '" + m_scope + "'"),
			msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));

			PanicMode();
			return;
		}
	}
}

void Compiler::SyntaxRead::EofError(std::string description) {
	m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekPrevToken()->GetLine(),
	msclr::interop::marshal_as<String^>(description), msclr::interop::marshal_as<String^>("Got-> 'eof'"));

	m_syntaxAnalyzer->EraseState();
}

void Compiler::SyntaxRead::PanicMode() {
	Token* currentToken = m_lexAnalyzer->PeekCurrentToken();

	if (!currentToken) {
		EofError("Error: Unexpected Token in Read in '" + m_scope + "'");
		return;
	}
	while (currentToken && currentToken->GetLexem() != ";" && currentToken->GetLexem() != "}") {
		currentToken = m_lexAnalyzer->GetNextToken();
	}
	if (!currentToken) {
		EofError("Error: Unexpected Token in Read in '" + m_scope + "'");
		return;
	}
	if (currentToken->GetLexem() == ";") {
		m_lexAnalyzer->GetNextToken();
	}
	m_syntaxAnalyzer->EraseState();
}

Compiler::SyntaxRead::SyntaxRead(LexAnalyzer* lexic, SyntaxAnalyzer* syntax, std::string scope)
: SyntaxState(lexic, syntax) {
	m_isDimension = false;
	m_isReturning = false;
	m_scope = scope;
}

Compiler::SyntaxRead::~SyntaxRead() {

}

