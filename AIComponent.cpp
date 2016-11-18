#include "stdafx.h"
#include "AIComponent.h"
#include "KUF3Math.h"
#include "KUF3Struct.h"
#include "Entity.h"

//-----------------------------------------------------------------------------------------------------------
BEGIN_DO_ATTR(CAIComponent)

CUSTOM_ATT_ITEM("Target", EntityPtr, SetTarget, GetTarget)

DO_ITEM("OutOfScreen", DoOutOfScreen)

END_DO_ATTR
//-----------------------------------------------------------------------------------------------------------
CAIComponent::CAIComponent(EntityPtr pEntity, const dynamic& values)
: CComponentBase(pEntity)
, m_bIsReady(false)
{
	m_State["AIComponent"].setClass(kaguya::UserdataMetatable<CAIComponent>()
		.setConstructors<CAIComponent(EntityPtr, const dynamic&)>()
		.addFunction("Ready", &CAIComponent::Ready)
		.addFunction("Chase", &CAIComponent::Chase)
		.addFunction("RangeCheck", &CAIComponent::RangeCheck)
		.addFunction("AttackToTarget", &CAIComponent::AttackWarningToTarget)
		.addFunction("ChangeRandomDir", &CAIComponent::ChangeRandomDir)
		.addFunction("ReturnBack", &CAIComponent::ReturnBack)
		.addFunction("IsUnderAttack", [&]() -> bool
	{
		return m_pEntity->GetAttr<bool>("IsInvincible");
	})
		.addFunction("SetInvincible", [&](CAIComponent* self, bool bIsInvincible)
	{
		m_pEntity->SetAttr("IsInvincible", bIsInvincible);
	})
		.addFunction("SetColor", [&](CAIComponent* self, int iColor)
	{
		m_pEntity->SetAttr("Color_Int", iColor);
	})
		.addFunction("SetSpeed", [&](CAIComponent* self, float fSpeed)
	{
		m_pEntity->SetAttr("Speed", fSpeed);
	})
		.addFunction("DirectAttack", [&](CAIComponent* self, const std::string& strAttackName)
	{
		m_pEntity->Do("DirectAttack", strAttackName);
	})
		.addFunction("DelayAttack", [&](CAIComponent* self, const std::string& strAttackName)
	{
		m_pEntity->Do("DelayAttack", strAttackName);
	})
		);

	m_State["AIClass"] = this;

	std::string strFileName = "lua/";
	strFileName += boost::any_cast<std::string>(values.at("Script"));
	m_State.dofile(strFileName);
}
//-----------------------------------------------------------------------------------------------------------
bool CAIComponent::Ready(float fElapsedTime)
{
	if (m_bIsReady)
		return true;

	float fAlpha = m_pEntity->GetAttr<float>("Alpha");

	fAlpha = fAlpha + fElapsedTime * 0.3f;

	if (fAlpha > 1.f)
	{
		m_pEntity->Do("SetTargetPos", RandomPoint());
		m_pEntity->SetAttr("IsInvincible", false);
		fAlpha = 1.f;
		m_bIsReady = true;
	}

	m_pEntity->SetAttr("Alpha", fAlpha);

	return false;
}
//-----------------------------------------------------------------------------------------------------------
void CAIComponent::Chase()
{
	if (m_pTargetEntity.lock() == nullptr)
		return;

	m_pEntity->Do("SetTargetPos", m_pTargetEntity.lock()->GetAttr<D2D1_POINT_2F>("Pos"));
}
//-----------------------------------------------------------------------------------------------------------
float CAIComponent::RangeCheck()
{
	if (m_pTargetEntity.lock() == nullptr)
		return 0.f;

	auto Point = m_pEntity->GetAttr<D2D1_POINT_2F>("Pos");
	auto TargetPoint = m_pTargetEntity.lock()->GetAttr<D2D1_POINT_2F>("Pos");

	float fRangeX = TargetPoint.x - Point.x;
	float fRangeY = TargetPoint.y - Point.y;

	return sqrt(fRangeX * fRangeX + fRangeY * fRangeY);
}
//-----------------------------------------------------------------------------------------------------------
void CAIComponent::AttackWarningToTarget()
{
	EntityPtr target = EntityPtr(m_pTargetEntity);
	auto Point = target->GetAttr<D2D1_POINT_2F>("Pos");

	AttackOption tAttackOption = { Point.x, Point.y, target->GetAttr<float>("Width"), target->GetAttr<float>("Height") };
	m_pEntity->Do("CreateWarningBox", tAttackOption);
}
//-----------------------------------------------------------------------------------------------------------
void CAIComponent::ChangeRandomDir()
{
	m_pEntity->Do("SetTargetPos", RandomPoint());
}