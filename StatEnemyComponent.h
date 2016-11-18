#pragma once

#include "Entity.h"

class CStatEnemyComponent : public CComponentBase
{
public:
	explicit CStatEnemyComponent(EntityPtr pEntity, const dynamic& values);
	virtual ~CStatEnemyComponent() {}

	DEFINE_DO_ATTR(CStatEnemyComponent)

private:
		GET_ATTR
		SET_ATTR
		DO_ACTION

		virtual COMPONENT_ID GetComponentID() const { return COMPONENT_ID::COM_STATENEMY; }

private:
	virtual void Update(float fElapsedTime);

private:
	bool DoIsDamaged(const boost::any& param);
	bool DoCollided(const boost::any& param)
	{
		EntityPtr otherTarget = boost::any_cast<EntityPtr>(param);
		otherTarget->Do("IsDamaged", m_pEntity->GetAttr<float>("Att"));
		return true;
	}
private:

	ATTACK_TYPE m_eAttType;
	float m_fCurHp;
	float m_fHp;
	float m_fAtt;
	bool m_bTempInvincible;
	float m_fTempInvincibleTime;
	bool m_bIsInvincible;
};