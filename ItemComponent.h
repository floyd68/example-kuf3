#pragma once

#include "Entity.h"

class CItemComponent : public CComponentBase
{
public:
	explicit CItemComponent(EntityPtr pEntity, const dynamic& values)
		: CComponentBase(pEntity)
		, m_fItemDisappearTime(0.f)
		, m_eItemID(ITEM_ID::ITEM_SPEEDUP) {}
	virtual ~CItemComponent() {}

	DEFINE_DO_ATTR(CItemComponent)

public:
	SET_ATTR
	GET_ATTR

	virtual COMPONENT_ID GetComponentID() const { return COMPONENT_ID::COM_ITEM; }
	virtual void Update(float fElapsedTime);

private:
	ITEM_ID m_eItemID;
	float m_fItemDisappearTime;
};
