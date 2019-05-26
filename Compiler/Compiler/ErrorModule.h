#pragma once
#include "pch.h"
#include "Headers.h"

using namespace System;

namespace Compiler
{
	public ref class ErrorModule
	{
		int m_numErrors;
		cli::array<String^>^ m_errors;

	public:
		ErrorModule();
		~ErrorModule();

		bool AddError(ErrorPhase phase, int lineNumber, String^ errorDescription, String^ errorLine);
		int GetErrorNumber();
		void ClearErrors();

		cli::array<String^>^ GetErrors();
	};
}