#include "pch.h"
#include "SyntaxFunction.h"
#include "SyntaxParameter.h"
#include "SyntaxBlock.h"
#include "SyntaxBegin.h"
#include "LexAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "SyntaxVar.h"

void Compiler::SyntaxFunction::CheckSyntax()
{
	Token* currentToken;

	if (!m_isReturning) {

		currentToken = m_lexAnalyzer->GetNextToken();
		m_line = currentToken->GetLine();

		if (!currentToken) {
			EofError("Error: Expected ID");
			return;
		}
		if (currentToken->GetType() == TokenType::ID) {
			m_name = currentToken->GetLexem();

			currentToken = m_lexAnalyzer->GetNextToken();
			if (!currentToken) {
				EofError("Error: Expected '('");
				return;
			}
			if (currentToken->GetLexem() == "(") {
				m_isReturning = true;
				m_syntaxAnalyzer->AddState(new SyntaxParameter(m_lexAnalyzer, m_syntaxAnalyzer, m_name));
				return;
			}
			else {
				m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
				"Error: Expected '('", 
				msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));

				PanicMode();
				return;
			}
		}
		else {
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
			"Error: Expected ID",
			msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));

			PanicMode();
			return;
		}
	}
	else {
		currentToken = m_lexAnalyzer->PeekCurrentToken();
		if (!currentToken) {
			EofError("Error: Expected ':'");
			return;
		}
		if (currentToken->GetLexem() == ":") {
			currentToken = m_lexAnalyzer->GetNextToken();
			if (!currentToken) {
				EofError("Error: Expected Data Type");
				return;
			}

			if (currentToken->GetLexem() == "int" || currentToken->GetLexem() == "float" || currentToken->GetLexem() == "bool"
			|| currentToken->GetLexem() == "string" || currentToken->GetLexem() == "void") {
				m_returnValue = currentToken->GetLexem();

				m_syntaxAnalyzer->GetSymbolTable()->AddSymbol(m_line, m_name, m_category, 0, m_returnValue, Global_Scope);

				currentToken = m_lexAnalyzer->GetNextToken();
				if (!currentToken) {
					EofError("Error: Expected '{'");
					return;
				}
				if (currentToken->GetLexem() == "{") {
					m_syntaxAnalyzer->EraseState();
					m_syntaxAnalyzer->AddState(new SyntaxBlock(m_lexAnalyzer, m_syntaxAnalyzer, m_name));
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
				m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
				"Error: Expected Data Type",
				msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));

				PanicMode();
				return;
			}
		}
		else {
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
			"Error: Expected ':'",
			msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));

			PanicMode();
			return;
		}
	}
}

void Compiler::SyntaxFunction::PanicMode()
{
	Token* currentToken = m_lexAnalyzer->PeekCurrentToken();

	if (!m_isReturning) {
		while (currentToken && currentToken->GetLexem() != "(" && currentToken->GetType() != TokenType::Keyword) {
			currentToken = m_lexAnalyzer->GetNextToken();
		}
		if (!currentToken) {
			EofError("Error: Expected '(' in function '" + m_name + "'");
			return;
		}
		if (currentToken->GetLexem() == "(") {
			m_isReturning = true;
			m_syntaxAnalyzer->AddState(new SyntaxParameter(m_lexAnalyzer, m_syntaxAnalyzer, m_name));
			return;
		}
		else if(currentToken->GetType() == TokenType::Keyword) {
			m_syntaxAnalyzer->EraseState();
			return;
		}
	}
	else {
		while (currentToken && currentToken->GetLexem() != "{" && currentToken->GetType() != TokenType::Keyword) {
			currentToken = m_lexAnalyzer->GetNextToken();
		}
		if (!currentToken) {
			EofError("Error: Expected '{' in function '" + m_name + "'");
			return;
		}
		if (currentToken->GetLexem() == "{") {
			m_syntaxAnalyzer->EraseState();
			m_syntaxAnalyzer->AddState(new SyntaxBlock(m_lexAnalyzer, m_syntaxAnalyzer, m_name));
		}
		else if (currentToken->GetType() == TokenType::Keyword) {
			m_syntaxAnalyzer->EraseState();
			return;
		}
	}
}

void Compiler::SyntaxFunction::EofError(std::string description)
{
	m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekPrevToken()->GetLine(),
	msclr::interop::marshal_as<String^>(description), msclr::interop::marshal_as<String^>("Got-> 'eof'"));

	m_syntaxAnalyzer->EraseState();
}

Compiler::SyntaxFunction::SyntaxFunction(LexAnalyzer* lexic, SyntaxAnalyzer* syntax) : SyntaxState(lexic, syntax) 
{
	m_name = "<undefined>";
	m_returnValue = "<undefined>";
	m_isReturning = false;
	m_category = SymbolCategory::Function;
}

Compiler::SyntaxFunction::~SyntaxFunction()
{
}
