#include "stdafx.h"
#include "StatEnemyComponent.h"
#include "GameLogicMgr.h"
#include "ObjectMgr.h"

//-----------------------------------------------------------------------------------------------------------
BEGIN_DO_ATTR(CStatEnemyComponent)

ATT_ITEM("Hp", float, m_fHp)
ATT_ITEM("CurHp", float, m_fCurHp)
ATT_ITEM("Att", float, m_fAtt)
ATT_ITEM("AttType", ATTACK_TYPE, m_eAttType)
ATT_ITEM("IsInvincible", bool, m_bIsInvincible)
ATT_ITEM("IsTempInvincible", bool, m_bTempInvincible)


DO_ITEM("OnCol", DoCollided)
DO_ITEM("IsDamaged", DoIsDamaged)


END_DO_ATTR

//-----------------------------------------------------------------------------------------------------------
CStatEnemyComponent::CStatEnemyComponent(EntityPtr pEntity, const dynamic& values)
: CComponentBase(pEntity)
, m_eAttType(ATTACK_TYPE::ATT_ONECELL)
, m_fCurHp(0.f)
, m_fHp(0.f)
, m_fAtt(0.f)
, m_fTempInvincibleTime(0.f)
, m_bTempInvincible(boost::any_cast<bool>(values.at("IsTempInvincible")))
, m_bIsInvincible(boost::any_cast<bool>(values.at("IsInvincible"))){}
//-----------------------------------------------------------------------------------------------------------
void CStatEnemyComponent::Update(float fElapsedTime)
{
	if (m_bTempInvincible)
	{
		m_fTempInvincibleTime += fElapsedTime;
		if (m_fTempInvincibleTime > 2.f)
		{
			m_bTempInvincible = false;
			m_fTempInvincibleTime = 0;
			m_pEntity->Do("Invincibility", false);
		}
	}
}
//-----------------------------------------------------------------------------------------------------------
bool CStatEnemyComponent::DoIsDamaged(const boost::any& param)
{
	if (m_bTempInvincible)
		return false;
	if (m_bIsInvincible)
		return false;
	m_bTempInvincible = true;
	m_pEntity->SetAttr("Alpha", 0.5f);

	float fAtt = boost::any_cast<float>(param);
	m_fCurHp -= fAtt;

	if (m_fCurHp <= 0.f)
	{
		m_fCurHp = 0.f;
		
		unsigned iRandom = rand() % 7;
		D2D1_POINT_2F Point = m_pEntity->GetAttr<D2D1_POINT_2F>("Pos");
		CGameLogicMgr::getSingleton()->CreateObject(CGameLogicMgr::getSingleton()->GetRadomItemTable()[iRandom], Point);

		CGameLogicMgr::getSingleton()->ScorePlus(100);
		m_pEntity->SetIsDelete();
	}
	return true;
}
//-----------------------------------------------------------------------------------------------------------
