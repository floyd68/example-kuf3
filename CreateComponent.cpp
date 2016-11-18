#include "stdafx.h"
#include "CreateComponent.h"
#include "KUF3Struct.h"
#include "GameLogicMgr.h"

//-----------------------------------------------------------------------------------------------------------
BEGIN_DO_ATTR(CCreateComponent)

ATT_ITEM("CreateObj", std::string, m_sCreateObj)

DO_ITEM("CreateAttackBox", DoCreateAttBox)
DO_ITEM("CreateWarningBox",DoWarningAttBox)
DO_ITEM("CreateCursor", DoCreateCursor)

END_DO_ATTR
//-----------------------------------------------------------------------------------------------------------
CCreateComponent::CCreateComponent(EntityPtr pEntity, const dynamic& values)
	: CComponentBase(pEntity)
	, m_sCreateObj(boost::any_cast<std::string>(values.at("CreateObj"))){}
//-----------------------------------------------------------------------------------------------------------
bool CCreateComponent::DoCreateAttBox(const boost::any& param)
{
	CreateBox(boost::any_cast<AttackOption>(param), m_sCreateObj.c_str());
	return true;
}
//-----------------------------------------------------------------------------------------------------------
bool CCreateComponent::DoWarningAttBox(const boost::any& param)
{
	CreateBox(boost::any_cast<AttackOption>(param), "WarningBox");
	return true;
}
//-----------------------------------------------------------------------------------------------------------
bool CCreateComponent::DoCreateCursor(const boost::any& param)
{
	CGameLogicMgr::getSingleton()->CreateObject("Cursor", boost::any_cast<D2D1_POINT_2F>(param));
	return true;
}
//-----------------------------------------------------------------------------------------------------------
void CCreateComponent::CreateBox(AttackOption& tAttackOption, const char* szName)
{
	EntityPtr pEntity = CGameLogicMgr::getSingleton()->CreateObject(szName, D2D1::Point2F(tAttackOption.fPosX, tAttackOption.fPosY));

	pEntity->SetAttr("Dir", tAttackOption.DirPt);
	pEntity->SetAttr("Att", m_pEntity->GetAttr<float>("Att"));
	pEntity->SetAttr("Width", tAttackOption.fWidth);
	pEntity->SetAttr("HeightGeo", tAttackOption.fHeight);
}