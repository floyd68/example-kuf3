#pragma once

#include "Entity.h"

class CStatPlayerComponent : public CComponentBase
{
public:
	explicit CStatPlayerComponent(EntityPtr pEntity, const dynamic& values);
	virtual ~CStatPlayerComponent() {}

	DEFINE_DO_ATTR(CStatPlayerComponent)

private:
		GET_ATTR
		SET_ATTR
		DO_ACTION

		virtual COMPONENT_ID GetComponentID() const { return COMPONENT_ID::COM_STATPLAYER; }

private:
	virtual void Update(float fElapsedTime);

private:
	bool DoIsDamaged(const boost::any& param);
	bool DoFullHp(const boost::any& param)
	{
		m_fCurHp = m_fHp;
		return true;
	}
	bool DoResetStat(const boost::any& param)
	{
		if (m_bAttUp)
		{
			m_pEntity->Do("Blink", false);
			m_bAttUp = false;
			m_fAttUpTime = 0.f;
			m_fAtt -= 5.f;
		}
		m_eAttType = ATTACK_TYPE::ATT_ONECELL;
		return true;
	}
	bool DoSendStatInfoToSever(const boost::any& param);
	bool DoCollided(const boost::any& param)
	{
		EntityPtr otherTarget = boost::any_cast<EntityPtr>(param);
		otherTarget->SetIsDelete();
		ITEM_ID eItemID = otherTarget->GetAttr<ITEM_ID>("Item");
		switch (eItemID)
		{
		case ITEM_ID::ITEM_HP:
		{
			m_fCurHp += 250.f;
			if (m_fCurHp > m_fHp)
				m_fCurHp = m_fHp;
			break;
		}
		case ITEM_ID::ITEM_ATT:
		{
			m_fAtt += 5.f;
			m_pEntity->Do("Blink", true);
			m_bAttUp = true;
			break;
		}
		case ITEM_ID::ITEM_TRICELL:
			m_eAttType = ATTACK_TYPE::ATT_TRIPLE;
			break;
		case ITEM_ID::ITEM_FOURCELL:
			m_eAttType = ATTACK_TYPE::ATT_FOURWAY;
			break;
		case ITEM_ID::ITEM_RAPIER:
			m_eAttType = ATTACK_TYPE::ATT_RAPIER;
			break;
		case ITEM_ID::ITEM_SHOTGUN:
			m_eAttType = ATTACK_TYPE::ATT_SHOTGUN;
			break;
		case ITEM_ID::ITEM_RIFLE:
			m_eAttType = ATTACK_TYPE::ATT_RIFLE;
			break;
		case ITEM_ID::ITEM_SPEEDUP:
			m_pEntity->Do("SpeedItem", eItemID);
			m_pEntity->Do("Blink", true);
			break;
		default:
			break;
		}
		return true;
	}
private:

	bool m_bAttUp;
	ATTACK_TYPE m_eAttType;
	float m_fCurHp;
	float m_fHp;
	float m_fAtt;
	float m_fAttUpTime;
	bool m_bTempInvincible;
	float m_fTempInvincibleTime;
	bool m_bIsInvincible;
};