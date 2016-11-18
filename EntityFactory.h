#pragma once

#include "Entity.h"

namespace EntityFactory
{
	struct ComsIDAndInitVal
	{
		std::string strComID;
		dynamic InitVal;
	};

	EntityPtr CreateEntity(const std::string& strName);

	typedef std::map<std::string, std::vector<ComsIDAndInitVal>> ENTITY_MAP;
	extern ENTITY_MAP m_mapComponentStruct;
};