#pragma once

#include "Entity.h"

struct AttackOption;

class CCreateComponent : public CComponentBase
{
public:
	explicit CCreateComponent(EntityPtr pEntity, const dynamic& values);
	virtual ~CCreateComponent() {}

	DEFINE_DO_ATTR(CCreateComponent)

private:
	GET_ATTR
	SET_ATTR
	DO_ACTION

	virtual COMPONENT_ID GetComponentID() const { return COMPONENT_ID::COM_CREATE; }

private:
	bool DoCreateAttBox(const boost::any& param);
	bool DoWarningAttBox(const boost::any& param);
	bool DoCreateCursor(const boost::any& param);
	void CreateBox(AttackOption& tAttackOption, const char* szName);
	
private:
	std::string m_sCreateObj;
};