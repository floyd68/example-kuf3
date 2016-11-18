#pragma once

#include "AIComponent.h"
#include "Entity.h"

class CAIComponentDragon : public CAIComponent
{
private:
	enum class DRAGON_AI
	{
		DRAGON_CHASE,
		DRAGON_BREATH
	};

public:
	CAIComponentDragon(EntityPtr pEntity, const dynamic& values)
		: CAIComponent(pEntity, values)
		, m_eDraginAI(DRAGON_AI::DRAGON_CHASE)
		, m_fBreathDelay(0.f) {}
	virtual ~CAIComponentDragon() {}

private:
	virtual COMPONENT_ID GetComponentID() const { return COMPONENT_ID::COM_AI_DRAGON; }

private:
	virtual void Update(float fElapsedTime);

private:
	void DragonShortBreath();
	void DragonLongBreath();
	void SetBreath();

private:
	DRAGON_AI m_eDraginAI;
	float m_fBreathDelay;
};