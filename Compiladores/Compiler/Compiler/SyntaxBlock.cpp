#include "pch.h"
#include "SyntaxFunctionCall.h"
#include "SyntaxLogicExpr.h"
#include "SyntaxAnalyzer.h"
#include "SyntaxAssign.h"
#include "SyntaxReturn.h"
#include "LexAnalyzer.h"
#include "SyntaxBlock.h"
#include "SyntaxWhile.h"
#include "SyntaxPrint.h"
#include "SyntaxRead.h"
#include "SyntaxVar.h"
#include "SyntaxIf.h"

void Compiler::SyntaxBlock::CheckSyntax()
{
	Token* currentToken;
		
	if(m_isReturning) {
		currentToken = m_lexAnalyzer->PeekCurrentToken();
	}
	else {
		currentToken = m_lexAnalyzer->GetNextToken();
	}
		
	while (currentToken && currentToken->GetLexem() != "}") {
		
		if (currentToken->GetLexem() == "if") {
			m_isReturning = true;
			m_syntaxAnalyzer->AddState(new SyntaxIf(m_lexAnalyzer, m_syntaxAnalyzer, m_scope));
			return;
		}
		else if (currentToken->GetLexem() == "switch") {
			m_isReturning = true;
			currentToken = m_lexAnalyzer->GetNextToken(); //Temporary
			return;
		}
		else if (currentToken->GetLexem() == "for") {
			m_isReturning = true;
			currentToken = m_lexAnalyzer->GetNextToken(); //Temporary
			return;
		}
		else if (currentToken->GetLexem() == "while") {
			m_isReturning = true;
			m_syntaxAnalyzer->AddState(new SyntaxWhile(m_lexAnalyzer, m_syntaxAnalyzer, m_scope));
			return;
		}
		else if (currentToken->GetLexem() == "print") {
			m_isReturning = true;
			m_syntaxAnalyzer->AddState(new SyntaxPrint(m_lexAnalyzer, m_syntaxAnalyzer, m_scope));
			return;
		}
		else if (currentToken->GetLexem() == "read") {
			m_isReturning = true;
			m_syntaxAnalyzer->AddState(new SyntaxRead(m_lexAnalyzer, m_syntaxAnalyzer, m_scope));
			return;
		}
		else if (currentToken->GetLexem() == "return" && m_scope != Main_Scope) {
			m_isReturning = true;
			m_syntaxAnalyzer->AddState(new SyntaxReturn(m_lexAnalyzer, m_syntaxAnalyzer, m_scope));
			return;
		}
		else if (currentToken->GetType() == TokenType::ID) {
			m_isReturning = true;
			
			Token* nextToken = m_lexAnalyzer->PeekNextToken();

			if (nextToken && nextToken->GetLexem() != "=") {
				m_syntaxAnalyzer->AddState(new SyntaxFunctionCall(m_lexAnalyzer, m_syntaxAnalyzer, m_scope));
			}
			else if (!nextToken) {
				m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
				msclr::interop::marshal_as<String^>("Error: Expected '=' or '(' after ID"),
				msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));

				PanicMode();

				currentToken = m_lexAnalyzer->PeekCurrentToken();
				if (!currentToken) {
					break;
				}
			}
			else {
				m_syntaxAnalyzer->AddState(new SyntaxAssign(m_lexAnalyzer, m_syntaxAnalyzer, m_scope));
			}
			return;
		}
		else if (currentToken->GetLexem() == "var") {
			m_isReturning = true;
			m_syntaxAnalyzer->AddState(new SyntaxVar(m_lexAnalyzer, m_syntaxAnalyzer, m_scope));
			return;
		}
		else if (currentToken->GetLexem() == ";") {
				currentToken = m_lexAnalyzer->GetNextToken();
		}
		else {
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
			msclr::interop::marshal_as<String^>("Error: Unexpected token in block"),
			msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));

			PanicMode();

			currentToken = m_lexAnalyzer->PeekCurrentToken();
			if (!currentToken) {
				break;
			}
		}
	}
	if (!currentToken) {
		EofError("Error: Expected '}'");
		return;
	}

	m_lexAnalyzer->GetNextToken();
	m_syntaxAnalyzer->EraseState();
}

void Compiler::SyntaxBlock::PanicMode() {

	Token* currentToken = m_lexAnalyzer->PeekCurrentToken();

	while (currentToken && currentToken->GetLexem() != ";" && currentToken->GetLexem() != "}") {
		currentToken = m_lexAnalyzer->GetNextToken();
	}

	if (currentToken->GetLexem() == ";") {
		m_lexAnalyzer->GetNextToken();
	}
}

void Compiler::SyntaxBlock::EofError(std::string description) {
	m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekPrevToken()->GetLine(),
	msclr::interop::marshal_as<String^>(description), msclr::interop::marshal_as<String^>("Got-> 'eof'"));

	m_syntaxAnalyzer->EraseState();
}

Compiler::SyntaxBlock::SyntaxBlock(LexAnalyzer* lexic, SyntaxAnalyzer* syntax, std::string scope) : SyntaxState(lexic, syntax) {
	m_scope = scope;
	m_isReturning = false;
}

Compiler::SyntaxBlock::~SyntaxBlock() {
}
