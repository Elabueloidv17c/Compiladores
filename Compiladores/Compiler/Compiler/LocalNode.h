#pragma once
#include "Headers.h"

namespace Compiler 
{
	class LocalNode
	{
		int					m_dimension;
		int					m_line;

		SymbolCategory		m_category;

		std::string			m_dataType;
		std::string			m_scope;
		std::string			m_name;

		void*				m_value;
		LocalNode*			m_nextNode;

	public:
		LocalNode(int line, std::string name, SymbolCategory category, int dimension, std::string dataType, std::string scope);

		int					GetDimension();
		int					GetLine();
		SymbolCategory		GetCategory();
		
		std::string			GetLineStr();
		std::string			GetDimensionStr();
		std::string			GetCategoryStr();
		std::string			GetDataType();
		std::string			GetScope();
		std::string			GetName();
		
		void				SetNextNode(LocalNode* localNode);
		LocalNode*			GetNextNode();
		bool				HasNextNode();
	};
};