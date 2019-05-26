#include "pch.h"
#include "LexAnalyzer.h"

#include "RelationalState.h"
#include "ArithmeticState.h"
#include "LogicalState.h"
#include "StringState.h"
#include "BeginState.h"
#include "FloatState.h"
#include "IntState.h"
#include "IDState.h"

void Compiler::BeginState::Read(const char* source, LexAnalyzer& analyzer)
{
	unsigned long currentChar = analyzer.GetCurrentChar();

	if (source[currentChar] == '_' || analyzer.IsAlpha(source[currentChar]))
	{
		//Posibilities: ID, Keyword
		analyzer.SetState(new IDState());
		return;
	}
	else if (analyzer.IsDigit(source[currentChar]))
	{
		//Posibilities: Int, Float
		analyzer.SetState(new IntState(false));
		return;
	}
	else if (source[currentChar] == '.')
	{
		//Posibilities: Float
		analyzer.SetState(new FloatState(source, analyzer));
		return;
	}
	else if (source[currentChar] == '=' || source[currentChar] == '!' || source[currentChar] == '>' || source[currentChar] == '<')
	{
		//Posibilities: "= Assign"	"! Unary Logical", Relational
		analyzer.SetState(new RelationalState(source, analyzer));
		return;
	}
	else if (source[currentChar] == '"')
	{
		//Posibilities: String
		analyzer.SetState(new StringState(source, analyzer));
		return;
	}
	else if (source[currentChar] == '+' || source[currentChar] == '-' || source[currentChar] == '/' ||
	source[currentChar] == '^' || source[currentChar] == '%' || source[currentChar] == '*')
	{
		//Posibilities: Arithmetic, Commentary, int, float
		analyzer.SetState(new ArithmeticState(source, analyzer));
		return;
	}
	else if (source[currentChar] == '|' || source[currentChar] == '&')
	{
		//Posibilities: Logical Operators
		analyzer.SetState(new LogicalState(source, analyzer));
		return;
	}
	else if (source[currentChar] == '{' || source[currentChar] == '}' || source[currentChar] == '(' || source[currentChar] == ')')
	{
		//Add grouping Token
		std::string token;
		token += source[currentChar];
		analyzer.AddToken(token, TokenType::Grouping, analyzer.GetCurrentLine());
	}
	else if (source[currentChar] == '[' || source[currentChar] == ']')
	{
		//Add Dimension Token
		std::string token;
		token += source[currentChar];
		analyzer.AddToken(token, TokenType::Dimension, analyzer.GetCurrentLine());
	}
	else if (source[currentChar] == ',' || source[currentChar] == ':' || source[currentChar] == ';')
	{
		//Add Separator Token
		std::string token;
		token += source[currentChar];
		analyzer.AddToken(token, TokenType::Separator, analyzer.GetCurrentLine());
	}
	else if (source[currentChar] == '\r')
	{
		analyzer.IncCurrentLine();
	}
	else if (source[currentChar] == ' ' || source[currentChar] == '\t' || source[currentChar] == '\n')
	{
		analyzer.IncCurrentChar();
		return;
	}
	else
	{
		if (source[currentChar] == '\0')
		{
			return;
		}

		std::string error;
		if (analyzer.GetTokens()->size())
		{
			error += analyzer.GetCurrentToken()->GetLexem();
		}

		error += source[currentChar];
		analyzer.AddError(ErrorPhase::Lexic, analyzer.GetCurrentLine(), _LEX_INVALID_CHAR, msclr::interop::marshal_as<String^>(error));
	}

	analyzer.IncCurrentChar();
}

Compiler::BeginState::BeginState()
{
}


Compiler::BeginState::~BeginState()
{
}
