#include "pch.h"
#include "SyntaxMain.h"
#include "SyntaxVar.h"
#include "SyntaxBlock.h"
#include "LexAnalyzer.h"
#include "SyntaxAnalyzer.h"

void Compiler::SyntaxMain::EofError(std::string description) {
	m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekPrevToken()->GetLine(),
	msclr::interop::marshal_as<String^>(description), msclr::interop::marshal_as<String^>("Got-> 'eof'"));

	m_syntaxAnalyzer->EraseState();
}

void Compiler::SyntaxMain::CheckSyntax() {

	Token* currentToken = m_lexAnalyzer->GetNextToken();

	if (currentToken && currentToken->GetLexem() == "(") {

		currentToken = m_lexAnalyzer->GetNextToken();
		if (currentToken && currentToken->GetLexem() == ")") {
			
			currentToken = m_lexAnalyzer->GetNextToken();
			if (currentToken && currentToken->GetLexem() == "{") {
				m_syntaxAnalyzer->EraseState();
				m_syntaxAnalyzer->AddState(new SyntaxBlock(m_lexAnalyzer, m_syntaxAnalyzer, Main_Scope));
				return;
			}
			else {
				if (!currentToken) {
					EofError("Error: Expected '{'");
					return;
				}
				m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
				"Error: Expected '{'",
				msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));

				PanicMode();
				return;
			}
		}
		else {
			if (!currentToken) {
				EofError("Error: Expected ')'");
				return;
			}

			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
			"Error: Expected ')'",
			msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));
			PanicMode();
			return;
		}
	}
	else {
		if (!currentToken) {
			EofError("Error: Expected '('");
			return;
		}
		m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
		"Error: Expected '('", 
		msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));

		PanicMode();
		return;
	}
}

void Compiler::SyntaxMain::PanicMode()
{
	Token* currentToken = m_lexAnalyzer->PeekCurrentToken();

	while (currentToken && currentToken->GetLexem() != "{" && currentToken->GetType() != TokenType::Keyword) {
		currentToken = m_lexAnalyzer->GetNextToken();
	}
	if (!currentToken) {
		EofError("Error: unexpected token in function 'main_function'");
		return;
	}
	if (currentToken->GetLexem() == "{") {
		m_syntaxAnalyzer->EraseState();
		m_syntaxAnalyzer->AddState(new SyntaxBlock(m_lexAnalyzer, m_syntaxAnalyzer, Main_Scope));
	}
	else if (currentToken->GetType() == TokenType::Keyword) {
		m_syntaxAnalyzer->EraseState();
		return;
	}
}

Compiler::SyntaxMain::SyntaxMain(LexAnalyzer* lexic, SyntaxAnalyzer* syntax) : SyntaxState(lexic, syntax) {
}

Compiler::SyntaxMain::~SyntaxMain() {
}
