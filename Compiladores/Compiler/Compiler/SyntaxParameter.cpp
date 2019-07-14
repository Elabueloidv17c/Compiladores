#include "pch.h"
#include "SyntaxParameter.h"
#include "SyntaxBegin.h"
#include "LexAnalyzer.h"
#include "SyntaxAnalyzer.h"

void Compiler::SyntaxParameter::CheckSyntax()
{
	Token* currentToken = m_lexAnalyzer->GetNextToken();

	if (!currentToken) {
		EofError("Error: Expected ID or ')'");
		return;
	}

	if (!currentToken->GetLexem().compare(")")) {
		if (m_isRepeating) {
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
			msclr::interop::marshal_as<String^>("Error: Expected 'ID'"),
			msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));
		}

		//Parameter block is empty
		m_lexAnalyzer->GetNextToken();
		m_syntaxAnalyzer->EraseState();
		return;
	}

	bool isIDFound = false;

	while (currentToken) {
		if (currentToken->GetType() == TokenType::ID) {
			isIDFound = true;

			//Add the current name to the vector
			VarData data;
			data.name = currentToken->GetLexem();
			data.line = currentToken->GetLine();

			currentToken = m_lexAnalyzer->GetNextToken();
			if (!currentToken) {
				EofError("Error: Expected '[', ':' or ','");
				return;
			}
			if (currentToken->GetLexem() == "[") {
				currentToken = m_lexAnalyzer->GetNextToken();
				if (!currentToken) {
					EofError("Error: Expected positive int");
					return;
				}

				if (currentToken->GetType() == TokenType::Int && std::atoi(&currentToken->GetLexem()[0]) >= 0) {
					data.length = std::atoi(&currentToken->GetLexem()[0]);

					currentToken = m_lexAnalyzer->GetNextToken();
					if (!currentToken) {
						EofError("Error: Expected ']'");
						return;
					}
					if (currentToken->GetLexem() != "]") {
						m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
							msclr::interop::marshal_as<String^>("Error: Expected ']'"),
							msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));

						PanicMode();
						return;
					}

					currentToken = m_lexAnalyzer->GetNextToken();
					if (!currentToken) {
						EofError("Error: Expected ID or ','");
						return;
					}
				}
				else {
					m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
						msclr::interop::marshal_as<String^>("Error: Expected positive int"),
						msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));

					PanicMode();
					return;
				}

			}
			else {
				data.length = 0;
			}
			m_names.push_back(data);

			if (currentToken->GetLexem() == ":") {
				//We finished reading the id's, it's time to read the type
				break;
			}
			if (currentToken->GetLexem() == ",") {
				//We get a "," advance to the next token to look for other variable
				currentToken = m_lexAnalyzer->GetNextToken();
				if (!currentToken) {
					EofError("Error: ID");
					return;
				}
			}
			else {
				m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekPrevToken()->GetLine(),
				msclr::interop::marshal_as<String^>("Error: Expected '[', ':' or ','"),
				msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));

				PanicMode();
				return;
			}
		}
		else {
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
				msclr::interop::marshal_as<String^>("Error: Expected ID"),
				msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));

			PanicMode();
			return;
		}
	}
	
	if (!currentToken) {
		EofError("Error: Expected ':'");
		return;
	}
	if (!isIDFound) {
		m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
		msclr::interop::marshal_as<String^>("Error: Expected ID"),
		msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));

		PanicMode();
		return;
	}

	//Add All names to the Symbol Table
	currentToken = m_lexAnalyzer->GetNextToken();
	if (!currentToken) {
		EofError("Error: Expected DataType");
		return;
	}

	if (currentToken->GetLexem() == "int" || currentToken->GetLexem() == "float" || currentToken->GetLexem() == "bool"
	|| currentToken->GetLexem() == "string") {

		m_dataType = currentToken->GetLexem();

		for (int i = 0; i < m_names.size(); i++) {
			m_syntaxAnalyzer->GetSymbolTable()->AddSymbol(m_names[i].line, m_names[i].name, m_category,
			m_names[i].length, m_dataType, m_function);
		}

		currentToken = m_lexAnalyzer->GetNextToken();
		if (!currentToken) {
			EofError("Error: Expected ';' or ')'");
			return;
		}

		if (currentToken->GetLexem() == ";") {
			m_isRepeating = true;
			m_names.clear();
			CheckSyntax();
		}
		else if(currentToken->GetLexem() == ")") {
			m_lexAnalyzer->GetNextToken();
			m_syntaxAnalyzer->EraseState();
			return;
		}
		else {
			m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
			msclr::interop::marshal_as<String^>("Error: Expected ')' or ';'"),
			msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));

			PanicMode();
			return;
		}
	}
	else {
		m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekCurrentToken()->GetLine(),
		msclr::interop::marshal_as<String^>("Error: Invalid Datatype"), 
		msclr::interop::marshal_as<String^>("Got-> '" + m_lexAnalyzer->PeekCurrentToken()->GetLexem() + "'"));

		PanicMode();
		return;
	}
}

void Compiler::SyntaxParameter::PanicMode()
{
	Token* currentToken = m_lexAnalyzer->PeekCurrentToken();

	if (!currentToken) {
		EofError("Error: Expected ')' or ';'");
		return;
	}
	while (currentToken && currentToken->GetLexem() != ")") {
			currentToken = m_lexAnalyzer->GetNextToken();
	}
	if (!currentToken) {
		EofError("Error: Expected ')'");
		return;
	}
	if (currentToken && currentToken->GetLexem() == ")") {
		m_lexAnalyzer->GetNextToken();
	}
	m_syntaxAnalyzer->EraseState();
}

void Compiler::SyntaxParameter::EofError(std::string description) {
	m_lexAnalyzer->AddError(ErrorPhase::Syntactic, m_lexAnalyzer->PeekPrevToken()->GetLine(),
	msclr::interop::marshal_as<String^>(description), msclr::interop::marshal_as<String^>("Got-> 'eof'"));

	m_syntaxAnalyzer->EraseState();
}

Compiler::SyntaxParameter::SyntaxParameter(LexAnalyzer* lexic, SyntaxAnalyzer* syntax, std::string function) 
: SyntaxState(lexic, syntax) {
	m_isRepeating = false;
	m_function = function;
	m_category = SymbolCategory::Parameter;
}

Compiler::SyntaxParameter::~SyntaxParameter() {
}