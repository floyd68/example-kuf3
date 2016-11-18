#include "stdafx.h"
#include "AttackComponent.h"
#include "KUF3Struct.h"

//-----------------------------------------------------------------------------------------------------------
BEGIN_DO_ATTR(CAttackComponent)

DO_ITEM("DirectAttack", DoAtt)
DO_ITEM("DelayAttack", DoAttWarning)

END_DO_ATTR

struct st_AttBoxSizeOption
{
	float fWidth;
	float fHeight;
	float fDistance;
};
typedef std::vector<st_AttBoxSizeOption> VecAttTypeSize;

static std::map<std::string, VecAttTypeSize> s_mapAttrTypeSizeTable =
{
	{ "ATT_ONECELL", {
		{ 50.f, 50.f, 25.f },
	} },
	{ "ATT_TRIPLE", {
		{ 150.f, 50.f, 25.f },
	} },
	{ "ATT_FOURWAY", {
		{ 50.f, 50.f, 25.f },
	} },
	{ "ATT_RAPIER", {
		{ 50.f, 100.f, 75.f },
	} },
	{ "ATT_SHOTGUN", {
		{ 50.f, 50.f, 25.f },
		{ 150.f, 150.f, 100.f },
	} },
	{ "ATT_RIFLE", {
		{ 50.f, 250.f, 125.f },
	} },
	{ "ATT_FIRE", {
		{ 50.f, 50.f, 25.f },
		{ 150.f, 50.f, 50.f },
		{ 250.f, 50.f, 50.f },
	} },
	{ "ATT_ONEWAY", {
		{ 50.f, 750.f, 375.f },
	} },
};
//-----------------------------------------------------------------------------------------------------------
bool CAttackComponent::DoAtt(const boost::any& param)
{
	std::string strName = boost::any_cast<std::string>(param);

	auto iter = s_mapAttrTypeSizeTable.find(strName);

	if (iter == s_mapAttrTypeSizeTable.end())
		return false;

	AttackOption tAttackOption = GetAttackOption();

	tAttackOption.fPosX += tAttackOption.DirPt.x * (tAttackOption.fWidth * 0.5f);
	tAttackOption.fPosY += tAttackOption.DirPt.y * (tAttackOption.fHeight * 0.5f);

	auto AttBoxSize = iter->second;
	size_t nSize = AttBoxSize.size();

	for (size_t i = 0; i < nSize; ++i)
	{
		tAttackOption.fPosX += tAttackOption.DirPt.x * AttBoxSize[i].fDistance;
		tAttackOption.fPosY += tAttackOption.DirPt.y * AttBoxSize[i].fDistance;
		tAttackOption.fWidth = AttBoxSize[i].fWidth;
		tAttackOption.fHeight = AttBoxSize[i].fHeight;
		m_pEntity->Do("CreateAttackBox", tAttackOption);
	}

	if (strName == "ATT_RAPIER")
		m_pEntity->Do("RapierMove");

	return true;
}
//-----------------------------------------------------------------------------------------------------------
bool CAttackComponent::DoAttWarning(const boost::any& param)
{
	auto iter = s_mapAttrTypeSizeTable.find(boost::any_cast<std::string>(param));

	if (iter == s_mapAttrTypeSizeTable.end())
		return false;

	AttackOption tAttackOption = GetAttackOption();

	tAttackOption.fPosX += tAttackOption.DirPt.x * (tAttackOption.fWidth * 0.5f);
	tAttackOption.fPosY += tAttackOption.DirPt.y * (tAttackOption.fHeight * 0.5f);

	auto AttBoxSize = iter->second;
	size_t nSize = AttBoxSize.size();

	for (size_t i = 0; i < nSize; ++i)
	{
		tAttackOption.fPosX += tAttackOption.DirPt.x * AttBoxSize[i].fDistance;
		tAttackOption.fPosY += tAttackOption.DirPt.y * AttBoxSize[i].fDistance;
		tAttackOption.fWidth = AttBoxSize[i].fWidth;
		tAttackOption.fHeight = AttBoxSize[i].fHeight;
		m_pEntity->Do("CreateWarningBox", tAttackOption);
	}

	return true;
}
//-----------------------------------------------------------------------------------------------------------
bool CAttackComponent::DoAttAllWay(const boost::any& param)
{
	AttackOption tAttackOption = GetAttackOption();
	float fRange = boost::any_cast<float>(tAttackOption.fWidth);

	tAttackOption.fPosX += tAttackOption.fWidth;
	m_pEntity->Do("CreateWarningBox", tAttackOption);

	tAttackOption.fPosY -= tAttackOption.fHeight;
	m_pEntity->Do("CreateWarningBox", tAttackOption);

	tAttackOption.fPosX -= tAttackOption.fWidth;
	m_pEntity->Do("CreateWarningBox", tAttackOption);

	tAttackOption.fPosX -= tAttackOption.fWidth;
	m_pEntity->Do("CreateWarningBox", tAttackOption);

	tAttackOption.fPosY += tAttackOption.fHeight;
	m_pEntity->Do("CreateWarningBox", tAttackOption);

	tAttackOption.fPosY += tAttackOption.fHeight;
	m_pEntity->Do("CreateWarningBox", tAttackOption);

	tAttackOption.fPosX += tAttackOption.fWidth;
	m_pEntity->Do("CreateWarningBox", tAttackOption);

	tAttackOption.fPosX += tAttackOption.fWidth;
	m_pEntity->Do("CreateWarningBox", tAttackOption);
	return true;
}
//-----------------------------------------------------------------------------------------------------------
AttackOption CAttackComponent::GetAttackOption() const
{
	AttackOption tAttackOption;
	D2D1_POINT_2F Point = m_pEntity->GetAttr<D2D1_POINT_2F>("Pos");

	tAttackOption.fPosX = Point.x;
	tAttackOption.fPosY = Point.y;

	tAttackOption.DirPt = m_pEntity->GetAttr<D2D1_POINT_2F>("Dir");

	tAttackOption.fWidth = m_pEntity->GetAttr<float>("Width");
	tAttackOption.fHeight = m_pEntity->GetAttr<float>("Height");

	return tAttackOption;
}