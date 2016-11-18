#pragma once

#include "Entity.h"

class CCursorComponent : public CComponentBase
{
public:
	explicit CCursorComponent(EntityPtr pEntity, const dynamic& values)
		: CComponentBase(pEntity)
		, m_fScale(1.f) {}
	virtual ~CCursorComponent() {}

private:
	virtual COMPONENT_ID GetComponentID() const { return COMPONENT_ID::COM_CURSOR; }

private:
	virtual void Update(float fElapsedTime);

private:
	float m_fScale;
};