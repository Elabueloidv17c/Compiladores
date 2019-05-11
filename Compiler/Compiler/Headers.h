#pragma once
#include <string>
#include <stack>

namespace Compiler
{
	enum TokenType
	{
		LiteralConstant,
		LogicConstant,
		Assignation,
		Relational,
		Arithmetic,
		Delimiter,
		Dimension,
		Grouping,
		KeyWord,
		Symbol,
		Logic,
		Unary,
	};
}