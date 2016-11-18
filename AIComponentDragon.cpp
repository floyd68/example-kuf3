#include "stdafx.h"
#include "AIComponentDragon.h"
#include "KUF3Struct.h"

//-----------------------------------------------------------------------------------------------------------
void CAIComponentDragon::Update(float fElapsedTime)
{
	if (!m_bIsReady)
	{
		Ready(fElapsedTime);
		return;
	}

	m_fBreathDelay += fElapsedTime;

	switch (m_eDraginAI)
	{
	case DRAGON_AI::DRAGON_CHASE:
		Chase();

		if (m_fBreathDelay > 1.5f)
		{
			/*if (abs(m_fRangeX) < 100.f)
			{
				if (m_fRangeY < 0.f)
					m_pEntity->SetAttr("Direction", UNIT_DIRECTION::DIR_UP);
				else
					m_pEntity->SetAttr("Direction", UNIT_DIRECTION::DIR_DOWN);

				SetBreath();
			}
			else if (abs(m_fRangeY) < 100.f)
			{
				if (m_fRangeX < 0.f)
					m_pEntity->SetAttr("Direction", UNIT_DIRECTION::DIR_LEFT);
				else
					m_pEntity->SetAttr("Direction", UNIT_DIRECTION::DIR_RIGHT);

				SetBreath();
			}*/
		}
		break;
	case DRAGON_AI::DRAGON_BREATH:
		if (m_fBreathDelay > 1.f)
		{
			m_fBreathDelay = 0.f;
			m_eDraginAI = DRAGON_AI::DRAGON_CHASE;
		}
		break;
	}
}
//-----------------------------------------------------------------------------------------------------------
void CAIComponentDragon::DragonShortBreath()
{
	m_fBreathDelay = 0.f;

	auto eMyDir = m_pEntity->GetAttr<UNIT_DIRECTION>("Direction");
	auto Point = m_pEntity->GetAttr<D2D1_POINT_2F>("Pos");

	AttackOption tAttackOption = { Point.x, Point.y, 50.f, 50.f };

	switch (eMyDir)
	{
	case UNIT_DIRECTION::DIR_LEFT:
		tAttackOption.fPosX -= tAttackOption.fWidth * 2.f;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosX -= tAttackOption.fWidth;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosY -= tAttackOption.fHeight;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosY += tAttackOption.fHeight * 2.f;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosX -= tAttackOption.fWidth;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosY += tAttackOption.fHeight;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosY -= tAttackOption.fHeight * 2.f;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosY -= tAttackOption.fHeight;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosY -= tAttackOption.fHeight;
		m_pEntity->Do("CreateWarningBox", tAttackOption);
		break;
	case UNIT_DIRECTION::DIR_RIGHT:
		tAttackOption.fPosX += tAttackOption.fWidth * 2.f;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosX += tAttackOption.fWidth;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosY -= tAttackOption.fHeight;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosY += tAttackOption.fHeight * 2.f;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosX += tAttackOption.fWidth;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosY += tAttackOption.fHeight;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosY -= tAttackOption.fHeight * 2.f;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosY -= tAttackOption.fHeight;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosY -= tAttackOption.fHeight;
		m_pEntity->Do("CreateWarningBox", tAttackOption);
		break;
	case UNIT_DIRECTION::DIR_UP:
		tAttackOption.fPosY -= tAttackOption.fHeight * 2.f;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosY -= tAttackOption.fHeight;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosX -= tAttackOption.fWidth;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosX += tAttackOption.fWidth * 2.f;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosY -= tAttackOption.fHeight;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosX += tAttackOption.fWidth;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosX -= tAttackOption.fWidth * 2.f;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosX -= tAttackOption.fWidth;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosX -= tAttackOption.fWidth;
		m_pEntity->Do("CreateWarningBox", tAttackOption);
		break;
	case UNIT_DIRECTION::DIR_DOWN:
		tAttackOption.fPosY += tAttackOption.fHeight * 2.f;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosY += tAttackOption.fHeight;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosX -= tAttackOption.fWidth;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosX += tAttackOption.fWidth * 2.f;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosY += tAttackOption.fHeight;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosX += tAttackOption.fWidth;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosX -= tAttackOption.fWidth * 2.f;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosX -= tAttackOption.fWidth;
		m_pEntity->Do("CreateWarningBox", tAttackOption);

		tAttackOption.fPosX -= tAttackOption.fWidth;
		m_pEntity->Do("CreateWarningBox", tAttackOption);
		break;
	}
}
//-----------------------------------------------------------------------------------------------------------
const float fDragonBreathWidth = 50.f;
const float fDragonBreathHeight = 50.f;

void CAIComponentDragon::DragonLongBreath()
{
	m_fBreathDelay = 0.f;

	auto eMyDir = m_pEntity->GetAttr<UNIT_DIRECTION>("Direction");
	auto Point = m_pEntity->GetAttr<D2D1_POINT_2F>("Pos");

	AttackOption tAttackOption = { Point.x, Point.y, fDragonBreathWidth, fDragonBreathHeight };

	static std::pair<float, float> s_LongBreath[] = {
		{ -fDragonBreathWidth, 0.f },
		{ fDragonBreathWidth, 0.f },
		{ 0.f, -fDragonBreathHeight },
		{ 0.f, fDragonBreathHeight },
	};

	for (int i = 0; i < 15; ++i)
	{
		tAttackOption.fPosX += s_LongBreath[static_cast<int>(eMyDir)].first;
		tAttackOption.fPosY += s_LongBreath[static_cast<int>(eMyDir)].second;
		m_pEntity->Do("CreateWarningBox", tAttackOption);
	}
}
//-----------------------------------------------------------------------------------------------------------
void CAIComponentDragon::SetBreath()
{
	if (m_fTargetRange < 250.f)
		DragonShortBreath();
	else
		DragonLongBreath();
	m_eDraginAI = DRAGON_AI::DRAGON_BREATH;
}