#include "pch.h"
#include "ErrorModule.h"

Compiler::ErrorModule::ErrorModule()
{
	m_numErrors = 0;

	m_errors = gcnew cli::array <String^>(_MAX_ERRORS);

	for (unsigned int i = 0; i < _MAX_ERRORS; i++)
	{
		m_errors[i] = gcnew String("");
	}
}

Compiler::ErrorModule::~ErrorModule()
{
	throw gcnew System::NotImplementedException();
}

void Compiler::ErrorModule::ClearErrors()
{
	m_numErrors = 0;
}

bool Compiler::ErrorModule::AddError(ErrorPhase phase, int lineNumber, String^ errorDescription, String^ errorLine)
{
	if (m_numErrors <= _MAX_ERRORS)
	{
		m_errors->SetValue(String::Format("{0}: Line {1},    {2} - {3}", phase.ToString(), lineNumber, errorDescription, errorLine), m_numErrors);
		
		m_numErrors++;
		return true;
	}
	else
	{
		return false;
	}
}

cli::array<String^>^ Compiler::ErrorModule::GetErrors()
{
	return m_errors;
}

int Compiler::ErrorModule::GetErrorNumber()
{
	return m_numErrors;
}