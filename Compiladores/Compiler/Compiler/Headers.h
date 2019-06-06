#pragma once
#include "pch.h"
#include <msclr\marshal_cppstd.h>
#include <stdlib.h>
#include <iostream>
#include <ctype.h>
#include <string>
#include <vector>
#include <stack>
#include <map>

namespace Compiler
{
	#define _MAX_ERRORS				30							//Completed
	#define _LEX_INVALID_ID			"Invalid identifier"		//Completed
	#define _STRING_NOT_CLOSED		"String not closed"
	#define _COMMENT_NOT_CLOSED		"Comment not closed"

	#define _INVALID_OP_LOG			"Invalid logic operator"	//Completed
	#define _INVALID_FLOAT			"Invalid float"				//Completed
	#define _LEX_INVALID_CHAR		"Invalid character"			//Completed

	public enum class ErrorPhase
	{
		Global,
		Lexic,
		Syntactic,
		Semantic,
		CodeGeneration
	};

	public enum class TokenType
	{
		ID,
		Int,
		Float,
		String,
		Relational,
		Arithmetic,
		Logical,
		UnaryLogical,
		Negation,
		Separator,
		Assign,
		Dimension,
		Grouping,
		Keyword
	};
}