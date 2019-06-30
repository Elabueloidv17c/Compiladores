#pragma once
#include "windows.h"
#include "vcclr.h"
#include "Headers.h"
#include "ErrorModule.h"

namespace Compiler
{
	class SemanticAnalyzer
	{
	public:
		SemanticAnalyzer(ErrorModule^ errorModule);
		SemanticAnalyzer();
		~SemanticAnalyzer();
	};
}
