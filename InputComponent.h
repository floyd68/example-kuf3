#pragma once

#include "Entity.h"

class CInputComponent : public CComponentBase
{
public:
	explicit CInputComponent(EntityPtr pEntity, const dynamic& values);
	virtual ~CInputComponent() {}

	DEFINE_DO_ATTR(CInputComponent)

private:
	DO_ACTION

	virtual COMPONENT_ID GetComponentID() const { return COMPONENT_ID::COM_INPUT; }

private:
	bool DoInputDown(const boost::any& param);
	bool DoInputRButtonDown(const boost::any& param);
public:
};