#include "stdafx.h"
#include "StatPlayerComponent.h"
#include "GameLogicMgr.h"
#include "ObjectMgr.h"
#include "MyMain.h"
#include "SMsg.h"
#include "Session.h"
//-----------------------------------------------------------------------------------------------------------
BEGIN_DO_ATTR(CStatPlayerComponent)

ATT_ITEM("Hp", float, m_fHp)
ATT_ITEM("CurHp", float, m_fCurHp)
ATT_ITEM("Att", float, m_fAtt)
ATT_ITEM("AttType", ATTACK_TYPE, m_eAttType)
ATT_ITEM("IsInvincible", bool, m_bIsInvincible)
ATT_ITEM("IsTempInvincible", bool, m_bTempInvincible)


DO_ITEM("IsDamaged", DoIsDamaged)
DO_ITEM("FullHp", DoFullHp)
DO_ITEM("ResetStat", DoResetStat)
DO_ITEM("OnCol", DoCollided)
DO_ITEM("SendStatInfoToSever", DoSendStatInfoToSever)


END_DO_ATTR
//-----------------------------------------------------------------------------------------------------------
CStatPlayerComponent::CStatPlayerComponent(EntityPtr pEntity, const dynamic& values)
: CComponentBase(pEntity)
, m_bAttUp(false)
, m_eAttType(ATTACK_TYPE::ATT_ONECELL)
, m_fCurHp(0.f)
, m_fHp(0.f)
, m_fAtt(0.f)
, m_fAttUpTime(0.f)
, m_fTempInvincibleTime(0.f)
, m_bTempInvincible(boost::any_cast<bool>(values.at("IsTempInvincible")))
, m_bIsInvincible(boost::any_cast<bool>(values.at("IsInvincible"))){}
//-----------------------------------------------------------------------------------------------------------
void CStatPlayerComponent::Update(float fElapsedTime)
{
	if (m_bAttUp)
	{
		m_fAttUpTime += fElapsedTime;
		if (m_fAttUpTime > 10.f)
		{
			m_pEntity->Do("Blink", false);
			m_bAttUp = false;
			m_fAttUpTime = 0.f;
			m_fAtt -= 5.f;
		}
	}
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
bool CStatPlayerComponent::DoIsDamaged(const boost::any& param)
{
	if (m_bTempInvincible)
		return false;
	m_bTempInvincible = true;
	m_bIsInvincible = true;
	m_pEntity->SetAttr("Alpha", 0.5f);

	float fAtt = boost::any_cast<float>(param);
	m_fCurHp -= fAtt;

	if (m_fCurHp <= 0.f)
	{
		m_fCurHp = 0.f;

		// 플레이어 죽음처리
		m_pEntity->Do("DieColor", 0);
		CGameLogicMgr::getSingleton()->SetDied(true);
	}
	return true;
}
//-----------------------------------------------------------------------------------------------------------
bool  CStatPlayerComponent::DoSendStatInfoToSever(const boost::any& param)
{
	SMsgSend msg(static_cast<unsigned short>(PACKET_ID::PACKET_SEND_STAT_INFO));
	msg << m_pEntity->GetEntityID() << m_bAttUp << m_eAttType << m_fCurHp << m_fHp << m_fAtt << m_fAttUpTime << m_bTempInvincible << m_fTempInvincibleTime << m_bIsInvincible;
	CMyMain::getSingleton()->GetSessionPtr()->Send(msg);

	return true;
}