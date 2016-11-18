#include "stdafx.h"
#include "InputComponent.h"
#include "KUF3Struct.h"
#include "GameLogicMgr.h"

//-----------------------------------------------------------------------------------------------------------
BEGIN_DO_ATTR(CInputComponent)

DO_ITEM("InputDown", DoInputDown)
DO_ITEM("InputRbtDown", DoInputRButtonDown)
END_DO_ATTR
//-----------------------------------------------------------------------------------------------------------
CInputComponent::CInputComponent(EntityPtr pEntity, const dynamic& values)
: CComponentBase(pEntity) {}
//-----------------------------------------------------------------------------------------------------------
bool CInputComponent::DoInputDown(const boost::any& param)
{
	int iKey = boost::any_cast<int>(param);

	if (CGameLogicMgr::getSingleton()->GetIsDied())
	{
		if (iKey == VK_RETURN)
			CGameLogicMgr::getSingleton()->SetDied(false);
		return true;
	}

	if (CGameLogicMgr::getSingleton()->GetIsAllClear())
	{
		if (iKey == VK_RETURN)
			CGameLogicMgr::getSingleton()->SetStageClear();
		return true;
	}
	switch (iKey)
	{
	case VK_SPACE:
		static std::string s_AttackTable[] = {
			"ATT_ONECELL",
			"ATT_TRIPLE",
			"ATT_FOURWAY",
			"ATT_RAPIER",
			"ATT_SHOTGUN",
			"ATT_RIFLE",
		};
		ATTACK_TYPE attType = m_pEntity->GetAttr<ATTACK_TYPE>("AttType");
		m_pEntity->Do("DirectAttack", s_AttackTable[static_cast<int>(attType)]);
		m_pEntity->Do("StopMove", D2D1::Point2F(0,0));
		break;
	}
	return true;
}
//-----------------------------------------------------------------------------------------------------------
bool CInputComponent::DoInputRButtonDown(const boost::any& param)
{
	m_pEntity->Do("CreateCursor", param);
	m_pEntity->Do("RBtDown", param);
	return true;
}