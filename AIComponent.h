#pragma once

#include "Entity.h"
#include "kaguya/kaguya.hpp"

class CAIComponent : public CComponentBase
{
public:
	explicit CAIComponent(EntityPtr pEntity, const dynamic& values);
	virtual ~CAIComponent() {}

	DEFINE_DO_ATTR(CAIComponent)

private:
	GET_ATTR
	SET_ATTR
	DO_ACTION

	virtual COMPONENT_ID GetComponentID() const { return COMPONENT_ID::COM_AI; }

private:
	virtual void Update(float fElapsedTime)
	{
		m_State["Update"](fElapsedTime);
	}

private:
	bool Ready(float fElapsedTime);
	void RandomMove(float fElapsedTime);
	void Chase();
	float RangeCheck();
	void AttackWarningToTarget();
	void GetTarget(EntityPtr& target) const {};
	void SetTarget(EntityPtr target) { m_pTargetEntity = EntityWeakPtr(target); }
	bool DoOutOfScreen(const boost::any& param)
	{
		m_State["OutScreen"]();
		return true;
	}
	void ChangeRandomDir();
	void ReturnBack()
	{
		m_pEntity->SetAttr("Speed", m_pEntity->GetAttr<float>("Speed") * -1.f);
	}

private:
	bool m_bIsReady;
	EntityWeakPtr m_pTargetEntity;
	kaguya::State m_State;
};