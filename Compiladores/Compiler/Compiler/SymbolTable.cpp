#include "pch.h"
#include "SymbolTable.h"

std::vector<std::string> Compiler::SymbolTable::GetStrSymbols()
{
	std::vector<std::string> value;
	std::map<std::string, GlobalNode*>::iterator it;

	for (it = m_symbols.begin(); it != m_symbols.end(); it++)
	{
		std::string current;
		GlobalNode* gNode = it->second;

		if (gNode->GetCategory() != SymbolCategory::Undefined)
		{
			current += gNode->GetLineStr() + "\r" + gNode->GetName() + "\r" + gNode->GetCategoryStr() + "\r" +
			gNode->GetDimensionStr() + "\r" + gNode->GetDataType() + "\r" + gNode->GetScope();

			value.push_back(current);
			current = "";
		}

		LocalNode* lNode = gNode->GetLocalNode();

		while (lNode != nullptr)
		{
			current += lNode->GetLineStr() + "\r" + gNode->GetName() + "\r" + lNode->GetCategoryStr() + "\r" +
			lNode->GetDimensionStr() + "\r" + lNode->GetDataType() + "\r" + lNode->GetScope();

			value.push_back(current);
			current = "";

			lNode = lNode->GetNextNode();
		}
	}

	return value;
}

void Compiler::SymbolTable::AddSymbol(int line, std::string name, SymbolCategory category, int dimension, std::string dataType, std::string scope)
{
	if (!SymbolExists(name, category, scope))
	{
		if (category == SymbolCategory::Global)
		{
			// Verify that does not exist as function (call symbol Exist as specific word)
			if (!SymbolExists(name, SymbolCategory::Function, scope))
			{
				m_symbols.insert(std::pair<std::string, GlobalNode*>(name, new GlobalNode(line, name, category, dimension, dataType, scope)));
				return;
			}

			m_errorModule->AddError(ErrorPhase::Syntactic, line, msclr::interop::marshal_as<String^>(
			"Error: global var name already defined as function"), msclr::interop::marshal_as<String^>("Symbol: " + name));
		}
		else if (category == SymbolCategory::Function)
		{
			// Verify that does not exist as global var
			if (!SymbolExists(name, SymbolCategory::Global, scope))
			{
				if (!SymbolExists(name, SymbolCategory::Local, scope))
				{
					m_symbols.insert(std::pair<std::string, GlobalNode*>(name, new GlobalNode(line, name, category, dimension, dataType, scope)));
					return;
				}

				m_errorModule->AddError(ErrorPhase::Syntactic, line, msclr::interop::marshal_as<String^>(
				"Error: function name already defined as local var"), msclr::interop::marshal_as<String^>("Symbol: " + name));
				return;
			}

			m_errorModule->AddError(ErrorPhase::Syntactic, line, msclr::interop::marshal_as<String^>(
			"Error: function name already defined as global var"), msclr::interop::marshal_as<String^>("Symbol: " + name));
			return;
		}
		else if (category == SymbolCategory::Local)
		{
			// Verify that does not exist as parameter of the same function and named as function
			if (!SymbolExists(name, SymbolCategory::Parameter, scope))
			{
				if (!SymbolExists(name, SymbolCategory::Function, scope))
				{
					bool isLocalNull = true;

					if (m_symbols.find(name) == m_symbols.end())
					{
						m_symbols.insert(std::pair<std::string, GlobalNode*>(name, new GlobalNode(line, name, SymbolCategory::Undefined,
						dimension, dataType, scope)));

						auto it = m_symbols.rbegin();
						GlobalNode* gNode = it->second;
						LocalNode* lNode = gNode->GetLocalNode();

						while (lNode != nullptr)
						{
							isLocalNull = false;
							lNode = lNode->GetNextNode();
						}
						if (!isLocalNull)
						{
							lNode = new LocalNode(line, name, category, dimension, dataType, scope);
						}
						else
						{
							gNode->SetLocalNode(new LocalNode(line, name, category, dimension, dataType, scope));
						}
						return;
					}
					else
					{
						auto it = m_symbols.find(name);
						GlobalNode* gNode = it->second;
						LocalNode* lNode = gNode->GetLocalNode();

						while (lNode != nullptr)
						{
							isLocalNull = false;
							lNode = lNode->GetNextNode();
						}
						if (!isLocalNull)
						{
							lNode = new LocalNode(line, name, category, dimension, dataType, scope);
						}
						else
						{
							gNode->SetLocalNode(new LocalNode(line, name, category, dimension, dataType, scope));
						}
						return;
					}
				}

				m_errorModule->AddError(ErrorPhase::Syntactic, line, msclr::interop::marshal_as<String^>(
				"Error: local var name already defined as function"), msclr::interop::marshal_as<String^>("Symbol: " + name));
				return;
			}

			m_errorModule->AddError(ErrorPhase::Syntactic, line, msclr::interop::marshal_as<String^>(
			"Error: local var name already defined as parameter"), msclr::interop::marshal_as<String^>("Symbol: " + name));
			return;
		}
		else if (category == SymbolCategory::Parameter)
		{
			// Verify that does not exist as local var and not named as function
			if (!SymbolExists(name, SymbolCategory::Local, scope))
			{
				if (!SymbolExists(name, SymbolCategory::Function, scope))
				{
					bool isLocalNull = true;

					if (m_symbols.find(name) == m_symbols.end())
					{
						m_symbols.insert(std::pair<std::string, GlobalNode*>(name, new GlobalNode(line, name, SymbolCategory::Undefined,
						dimension, dataType, scope)));

						auto it = m_symbols.rbegin();
						GlobalNode* gNode = it->second;
						LocalNode* lNode = gNode->GetLocalNode();

						while (lNode != nullptr)
						{
							isLocalNull = false;
							lNode = lNode->GetNextNode();
						}
						if (!isLocalNull)
						{
							lNode = new LocalNode(line, name, category, dimension, dataType, scope);
						}
						else
						{
							gNode->SetLocalNode(new LocalNode(line, name, category, dimension, dataType, scope));
						}
						return;
					}
					else
					{
						auto it = m_symbols.find(name);
						GlobalNode* gNode = it->second;
						LocalNode* lNode = gNode->GetLocalNode();

						while (lNode != nullptr)
						{
							isLocalNull = false;
							lNode = lNode->GetNextNode();
						}
						if (!isLocalNull)
						{
							lNode = new LocalNode(line, name, category, dimension, dataType, scope);
						}
						else
						{
							gNode->SetLocalNode(new LocalNode(line, name, category, dimension, dataType, scope));
						}
						return;
					}
				}

				m_errorModule->AddError(ErrorPhase::Syntactic, line, msclr::interop::marshal_as<String^>(
				"Error: parameter name already defined as function"), msclr::interop::marshal_as<String^>("Symbol: " + name));
				return;
			}

			m_errorModule->AddError(ErrorPhase::Syntactic, line, msclr::interop::marshal_as<String^>(
			"Error: parameter name already defined as local variable"), msclr::interop::marshal_as<String^>("Symbol: " + name));
			return;
		}
	}
	else
	{
		m_errorModule->AddError(ErrorPhase::Syntactic, line, msclr::interop::marshal_as<String^>(
		"Error: Variable/Function already defined in the same scope"), msclr::interop::marshal_as<String^>("Symbol: " + name));
	}
}

bool Compiler::SymbolTable::SymbolExists(std::string symbol, SymbolCategory category, std::string function)
{
	if (m_symbols.find(symbol) != m_symbols.end())
	{
		if (category == SymbolCategory::Global || category == SymbolCategory::Function)
		{
			auto it = m_symbols.find(symbol);
			GlobalNode* gNode = it->second;

			if (gNode->GetCategory() == category)
			{
				return true;
			}
		}
		else
		{
			auto it = m_symbols.find(symbol);
			GlobalNode* gNode = it->second;
			LocalNode* lNode = gNode->GetLocalNode();

			while (lNode != nullptr)
			{
				if (lNode->GetCategory() == category && !(lNode->GetScope().compare(function)))
				{
					return true;
				}
				lNode = lNode->GetNextNode();
			}
		}
	}

	return false;
}

void Compiler::SymbolTable::Clear()
{
	m_symbols.erase(m_symbols.begin(), m_symbols.end());
}

Compiler::SymbolTable::SymbolTable(ErrorModule^ errors)
{
	m_errorModule = errors;
}

Compiler::SymbolTable::~SymbolTable()
{
}
