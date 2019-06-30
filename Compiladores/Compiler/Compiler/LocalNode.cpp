#include "pch.h"
#include "LocalNode.h"

Compiler::LocalNode::LocalNode(int line, std::string name, SymbolCategory category, int dimension, std::string dataType, std::string scope)
{
	m_nextNode = nullptr;
	m_dimension = dimension;
	m_category = category;
	m_dataType = dataType;
	m_scope = scope;
	m_line = line;
	m_name = name;
}

int	Compiler::LocalNode::GetDimension()
{
	return m_dimension;
}

int	Compiler::LocalNode::GetLine()
{
	return m_line;
}

Compiler::SymbolCategory Compiler::LocalNode::GetCategory()
{
	return m_category;
}

std::string	Compiler::LocalNode::GetLineStr()
{
	return std::to_string(m_line);
}

std::string	Compiler::LocalNode::GetDimensionStr()
{
	return std::to_string(m_dimension);
}

std::string	Compiler::LocalNode::GetCategoryStr()
{
	if (m_category == 0)
	{
		return "Undefined";
	}
	if (m_category == 1)
	{
		return "Global";
	}
	if (m_category == 2)
	{
		return "Local";
	}
	if (m_category == 3)
	{
		return "Function";
	}
	if (m_category == 4)
	{
		return "Parameter";
	}
}

std::string	Compiler::LocalNode::GetDataType()
{
	return m_dataType;
}

std::string	Compiler::LocalNode::GetScope()
{
	return m_scope;
}

std::string	Compiler::LocalNode::GetName()
{
	return m_name;
}

Compiler::LocalNode* Compiler::LocalNode::GetNextNode()
{
	return m_nextNode;
}

bool Compiler::LocalNode::HasNextNode()
{
	if (m_nextNode)
	{
		return  true;
	}
	return false;
}

void Compiler::LocalNode::SetNextNode(LocalNode* localNode)
{
	if (!m_nextNode)
	{
		m_nextNode = localNode;
	}
}