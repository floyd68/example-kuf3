#pragma once

#include "Entity.h"

class CAttackBoxComponent : public CComponentBase
{
public:
	explicit CAttackBoxComponent(EntityPtr pEntity, const dynamic& values)
		: CComponentBase(pEntity)
		, m_fAlpha(m_pEntity->GetAttr<float>("Alpha")) {}
	virtual ~CAttackBoxComponent() {}

private:
	virtual COMPONENT_ID GetComponentID() const { return COMPONENT_ID::COM_ENEMYATTACKBOX; }

private:
	virtual void Update(float fElapsedTime);

private:
	float m_fAlpha;
};