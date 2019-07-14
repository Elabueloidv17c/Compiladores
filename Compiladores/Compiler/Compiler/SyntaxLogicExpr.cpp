#include "pch.h"
#include "SyntaxFunctionCall.h"
#include "SyntaxLogicExpr.h"
#include "SyntaxAnalyzer.h"
#include "LexAnalyzer.h"

void Compiler::SyntaxLogicExpr::EofError(std::string description) {
	m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekPrevToken()->GetLine(),
	msclr::interop::marshal_as<String^>(description), msclr::interop::marshal_as<String^>("Got-> 'eof'"));

	m_syntaxAnalyzer->EraseState();
}

void Compiler::SyntaxLogicExpr::PanicMode() {

	Token* currentToken = m_lexAnalyzer->PeekCurrentToken();

	if (!currentToken) {
		EofError("Error: Unexpected Token in Logic expression in '" + m_scope + "'");
		return;
	}
	while (currentToken && currentToken->GetLexem() != ";" && currentToken->GetLexem() != "}") {
		currentToken = m_lexAnalyzer->GetNextToken();
	}
	if (!currentToken) {
		EofError("Error: Unexpected Token in Logic expression in '" + m_scope + "'");
		return;
	}
	if (currentToken->GetLexem() == ";") {
		m_lexAnalyzer->GetNextToken();
		m_isExitSet = true;
		m_syntaxAnalyzer->EraseState();
		return;
	}
	if (m_open > m_close) {
		m_isExitSet = true;

		for (int i = m_close; i < m_open; i++)
		{
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
			"Error: Term not closed, expected ')'",
			msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));
		}
		m_syntaxAnalyzer->EraseState();
		return;
	}
	m_syntaxAnalyzer->EraseState();
}

void Compiler::SyntaxLogicExpr::CheckSyntax() {
	
	Token* lastToken = m_lexAnalyzer->PeekPrevToken();
	if ((lastToken->GetLexem() == "," || lastToken->GetLexem() == "(") && m_open == 0 && m_close == 0
	&& !m_isOperatorFound && !m_isTermFound) {
		m_isLastInList = true;
	}
	
	//If function call was true last run -> set to false
	ProcessNot();
	ProcessTerm();
	//if is function call found -> return
	if (m_isExitSet) {
		return;
	}

	Token* currentToken = m_lexAnalyzer->PeekCurrentToken();

	if (!currentToken) {
		EofError("Error: Expected Operator in logic expression in '" + m_scope + "'");
		return;
	}
	Token* prevToken = m_lexAnalyzer->PeekPrevToken();
	if (currentToken->GetLexem() == ")" && (prevToken->GetLexem() == ")" || prevToken->GetType() == TokenType::Int 
	|| prevToken->GetType() == TokenType::Float || prevToken->GetType() == TokenType::String || prevToken->GetType() == TokenType::ID
	|| prevToken->GetLexem() == "bool" || prevToken->GetLexem() == "true" || prevToken->GetLexem() == "false")) {

		m_close++;

		if (m_close > m_open) {
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
			"Error: Unexpected token in logic expression",
			msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));

			PanicMode();
			return;
		}

		currentToken = m_lexAnalyzer->GetNextToken();
		return;
	}
	else if (currentToken->GetType() == TokenType::Arithmetic || currentToken->GetType() == TokenType::Relational
	|| currentToken->GetType() == TokenType::Logical) {
		m_isOperatorFound = true;

		currentToken = m_lexAnalyzer->GetNextToken();
		if (!currentToken) {
			EofError("Error: Expected Term in logic expression in '" + m_scope + "'");
			return;
		}
		CheckSyntax();
		return;
	}
	else {
		if (!m_isOperatorFound && !m_isTermFound) {
			if (m_open == m_close) {
				m_isExitSet = true;
				if (m_isCondition && m_open == 1) {
					m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
					"Error: Expected logic expression",
					msclr::interop::marshal_as<String^>("Got-> ')'"));
				}

				m_syntaxAnalyzer->EraseState();
				return;
			}
			while (m_open > m_close) {
				if (!currentToken) {
					EofError("Error: Expected ')' in logic expression in '" + m_scope + "'");
					return;
				}
				if (currentToken->GetLexem() == ")") {
					m_close++;
					currentToken = m_lexAnalyzer->GetNextToken();
				}
				else {
					m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
					"Error: Term not closed, expected ')'",
					msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));
					break;
				}
				m_isExitSet = true;
				m_syntaxAnalyzer->EraseState();
				return;
			}
		}
		if (currentToken->GetLexem() == ")") {
			if (m_close < m_open) {
				m_close++;
				return;
			}
			else {
				if(m_open != m_close){
					if (m_open > m_close) {
						for (int i = m_close; i < m_open; i++)
						{
							m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
								"Error: Term not closed, expected ')'",
								msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));
						}
					}
					else {
						for (int i = m_open; i < m_close; i++) {
							m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
							"Error: Term not closed, expected '('",
							msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));
						}
					}
				}
				m_isExitSet = true;
				m_syntaxAnalyzer->EraseState();
				return;
			}
		}
		else if (m_isTermFound) {
			if (!m_isOperatorFound && currentToken->GetLexem() != "," && currentToken->GetLexem() != ";" && currentToken->GetLexem() != "]"
			&& currentToken->GetLexem() != "{") {
				m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
				"Error: Expected Operand",
				msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));

				m_isExitSet = true;
				m_syntaxAnalyzer->EraseState();
				return;
			}
			if (m_open > m_close) {
				for (int i = m_close; i < m_open; i++)
				{
					m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
					"Error: Term not closed, expected ')'",
					msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));
				}
			}
			m_isExitSet = true;
			m_syntaxAnalyzer->EraseState();
			return;
		}

		m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
		"Error: Expected Operator",
		msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));

		PanicMode();
		return;
	}
}

void Compiler::SyntaxLogicExpr::ProcessTerm() {

	Token* currentToken = m_lexAnalyzer->PeekCurrentToken();
	if (!currentToken) {
		EofError("Error: Expected Term in logic expression in '" + m_scope + "'");
		return;
	}
	if (currentToken->GetLexem() == "(") {
		m_open++;
		currentToken = m_lexAnalyzer->GetNextToken();
		if (!currentToken) {
			EofError("Error: Expected Term in logic expression in '" + m_scope + "'");
			return;
		}
		CheckSyntax();
	}
	else if (currentToken->GetType() == TokenType::Int || currentToken->GetType() == TokenType::Float 
	|| currentToken->GetType() == TokenType::String || currentToken->GetType() == TokenType::ID
	|| currentToken->GetLexem() == "bool" || currentToken->GetLexem() == "true" || currentToken->GetLexem() == "false") {
		
		m_isTermFound = true;

		//Token* nextToken = m_lexAnalyzer->PeekNextToken();
		//
		//if (!nextToken) {
		//	EofError("Error: UReached End of file reading term in '" + m_scope + "'");
		//	PanicMode();
		//	return;
		//}
		//if (currentToken->GetType() == TokenType::ID && nextToken->GetLexem() == "(") {
		//	m_syntaxAnalyzer->AddState(new SyntaxFunctionCall(m_lexAnalyzer, m_syntaxAnalyzer, m_scope));
		//	return;
		//}

		currentToken = m_lexAnalyzer->GetNextToken();
	}
	else if (currentToken->GetLexem() == ")") {
		m_close++;
		Token* prevToken = m_lexAnalyzer->PeekPrevToken();
		if (prevToken->GetType() == TokenType::Arithmetic || prevToken->GetType() == TokenType::Relational
		|| prevToken->GetType() == TokenType::Logical) {
			
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
			"Error: Expected Term",
			msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));

			PanicMode();
			return;
		}
		currentToken = m_lexAnalyzer->GetNextToken();
	}
	else {
		m_lexAnalyzer->AddError(ErrorPhase::Syntactic, currentToken->GetLine(),
		"Error: Expected Term",
		msclr::interop::marshal_as<String^>("Got-> '" + currentToken->GetLexem() + "'"));

		PanicMode();
		return;
	}
}

void Compiler::SyntaxLogicExpr::ProcessNot() {
	Token* currentToken = m_lexAnalyzer->PeekCurrentToken();

	if (currentToken && currentToken->GetType() == TokenType::UnaryLogical) {
		m_lexAnalyzer->GetNextToken();
	}
}

Compiler::SyntaxLogicExpr::SyntaxLogicExpr(LexAnalyzer * lexic, SyntaxAnalyzer * syntax, std::string scope, bool isCondition)
: SyntaxState(lexic, syntax) {
	m_scope = scope;

	m_close = 0;
	m_open = 0;

	m_isTermFound = false;
	m_isExitSet = false;
	m_isOperatorFound = false;
	m_isLastInList = false;
	m_isCondition = isCondition;
}

Compiler::SyntaxLogicExpr::~SyntaxLogicExpr() {

}
