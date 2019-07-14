#include "pch.h"
#include "GlobalNode.h"

Compiler::GlobalNode::GlobalNode(int line, std::string name, SymbolCategory category, int dimension, std::string dataType, std::string scope)
{
	m_localNode = nullptr;
	m_dimension = dimension;
	m_category = category;
	m_dataType = dataType;
	m_scope = scope;
	m_line = line;
	m_name = name;
}

int	Compiler::GlobalNode::GetDimension()
{
	return m_dimension;
}

int	Compiler::GlobalNode::GetLine()
{
	return m_line;
}

Compiler::SymbolCategory Compiler::GlobalNode::GetCategory()
{
	return m_category;
}

std::string	Compiler::GlobalNode::GetLineStr()
{
	return std::to_string(m_line);
}

std::string	Compiler::GlobalNode::GetDimensionStr()
{
	return std::to_string(m_dimension);
}

std::string	Compiler::GlobalNode::GetCategoryStr()
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

std::string	Compiler::GlobalNode::GetDataType()
{
	return m_dataType;
}

std::string	Compiler::GlobalNode::GetScope()
{
	return m_scope;
}

std::string	Compiler::GlobalNode::GetName()
{
	return m_name;
}

Compiler::LocalNode* Compiler::GlobalNode::GetLocalNode()
{
	return m_localNode;
}

void Compiler::GlobalNode::SetLocalNode(LocalNode* localNode)
{
	LocalNode* nodeExplorer = m_localNode;

	if (!m_localNode)
	{
		m_localNode = localNode;
		return;
	}

	while (nodeExplorer->HasNextNode())
	{
		nodeExplorer->GetNextNode();
	}

	nodeExplorer->SetNextNode(localNode);
}

void Compiler::GlobalNode::SetLine(int line)
{
	m_line = line;
}

void Compiler::GlobalNode::SetDimension(int dimension)
{
	m_dimension = dimension;
}

void Compiler::GlobalNode::SetCategory(SymbolCategory category)
{
	m_category = category;
}

void Compiler::GlobalNode::SetDataType(std::string dataType)
{
	m_dataType = dataType;
}

void Compiler::GlobalNode::SetScope(std::string scope)
{
	m_scope = scope;
}

void Compiler::GlobalNode::SetName(std::string name)
{
	m_name = name;
}