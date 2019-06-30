#pragma once

#pragma once
#include "Headers.h"
#include "LocalNode.h"

namespace Compiler 
{
	class GlobalNode
	{
	private:

		int					m_dimension;
		int					m_line;

		SymbolCategory		m_category;

		std::string			m_dataType;
		std::string			m_scope;
		std::string			m_name;

		void*				m_value;
		LocalNode*			m_localNode;
		LocalNode*			m_nodeExplorer;

	public:
		GlobalNode(int line, std::string name, SymbolCategory category, int dimension, std::string dataType, std::string scope);

		int					GetDimension();
		int					GetLine();
		SymbolCategory		GetCategory();

		std::string			GetLineStr();
		std::string			GetDimensionStr();
		std::string			GetCategoryStr();
		std::string			GetDataType();
		std::string			GetScope();
		std::string			GetName();


		void				SetLine(int line);
		void				SetDimension(int dimension);
		void				SetCategory(SymbolCategory category);
		void				SetDataType(std::string dataType);
		void				SetScope(std::string scope);
		void				SetName(std::string name);

		void				SetLocalNode(LocalNode* localNode);
		LocalNode*			GetLocalNode();
	};

};