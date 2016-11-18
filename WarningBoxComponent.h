#pragma once

#include "Entity.h"

class CWarningBoxComponent : public CComponentBase
{
public:
	explicit CWarningBoxComponent(EntityPtr pEntity, const dynamic& values)
		: CComponentBase(pEntity)
		, m_fDeleteTime(0.f) {}
	virtual ~CWarningBoxComponent() {}

private:
	virtual COMPONENT_ID GetComponentID() const { return COMPONENT_ID::COM_WARNINGBOX; }

private:
	virtual void Update(float fElapsedTime);

private:
	float m_fDeleteTime;
};