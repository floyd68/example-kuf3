#include "stdafx.h"
#include "ColliderComponent.h"
#include "ObjectMgr.h"
#include "KUF3Math.h"

//-----------------------------------------------------------------------------------------------------------
BEGIN_DO_ATTR(CColliderComponent)

ATT_ITEM("ColObjType", OBJ_TYPE, m_ColObjType)

END_DO_ATTR

//-----------------------------------------------------------------------------------------------------------
CColliderComponent::CColliderComponent(EntityPtr pEntity, const dynamic& values)
	: CComponentBase(pEntity)
 	, m_ColObjType(boost::any_cast<OBJ_TYPE>(values.at("ColObjType"))){}
//-----------------------------------------------------------------------------------------------------------

void CColliderComponent::Update(float fElapsedTime)
{
	for (vCol::iterator iter = m_vecCol.begin(); iter != m_vecCol.end();)
	{
		if (iter->expired() || !IsCollided(iter->lock()))
		{
			m_pEntity->Do("CollideOut", iter->lock()); //충돌 끝
			iter = m_vecCol.erase(iter);
		}
		else
			++iter;
	}

	for (vCol::iterator iter = m_vecCol.begin(); iter != m_vecCol.end(); ++iter)
	{
		m_pEntity->Do("OnCol", iter->lock() );
	}


	auto check_collide = [this](EntityPtr pEntityOther)
	{
		IsInCollided(pEntityOther);
	};
	CObjectMgr::getSingleton()->for_each_entities(check_collide);

}
//-----------------------------------------------------------------------------------------------------------
bool CColliderComponent::IsCollided(EntityPtr otherTarget)
{
	if (CollideEntity(otherTarget))
		return true;
	return false;
}

//-----------------------------------------------------------------------------------------------------------
bool CColliderComponent::IsInCollided(EntityPtr otherTarget)
{
	if (m_ColObjType != otherTarget->GetObjType())
		return false;
	for (auto Colvec : m_vecCol)
	{
		if (Colvec.lock() == otherTarget)
			return false;
	}
	if (IsCollided(otherTarget))
	{
		m_pEntity->Do("BeginCol", otherTarget); //충돌 시작
		m_vecCol.push_back(otherTarget);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------------------
BOOL CColliderComponent::CollideEntity(EntityPtr otherTarget)
{
	RENDER_VALUE rValue = m_pEntity->GetAttr<RENDER_VALUE>("Shape");
	RENDER_VALUE rOtherValue = otherTarget->GetAttr<RENDER_VALUE>("Shape");

	if (rValue == RENDER_VALUE::E_RECT && rOtherValue == RENDER_VALUE::E_RECT)
		return OBBCol(otherTarget);
	else if (rValue == RENDER_VALUE::E_CIRCLE && rOtherValue == RENDER_VALUE::E_CIRCLE)
		return CircleCollide(otherTarget);
	else
	{
		switch (rOtherValue)
		{
		case RENDER_VALUE::E_CIRCLE:
			if (DifferentCollide(otherTarget, m_pEntity->shared_from_this()))
				return OBBCol(otherTarget);
			return FALSE;
		case RENDER_VALUE::E_RECT:
			if(DifferentCollide(m_pEntity->shared_from_this(), otherTarget))
				return OBBCol(otherTarget);
			return FALSE;
		default:
			return FALSE;
		}
	}
}
//-----------------------------------------------------------------------------------------------------------
BOOL CColliderComponent::CircleCollide(EntityPtr otherTarget)
{
	D2D1_POINT_2F Point;
	float fRadius = 0.f;
	GetCircleValues(m_pEntity->shared_from_this(), Point, fRadius);

	D2D1_POINT_2F OtherPoint;
	float fOtherfRadius = 0.f;
	GetCircleValues(otherTarget, OtherPoint, fOtherfRadius);

	//피타고라스 정리 두원 센터 사이에 거리에따라 판별
	float a = Point.x - OtherPoint.x;
	float b = Point.y - OtherPoint.y;
	float c = fRadius + fOtherfRadius;
	if (a * a + b * b <= c *c)
		return TRUE;
	return FALSE;
}
//-----------------------------------------------------------------------------------------------------------
bool CColliderComponent::CircleColBetweenDifShape(EntityPtr CircleTarget, EntityPtr RectTarget)
{
	D2D1_POINT_2F Point;
	float fRadius = 0.f;
	GetCircleValues(CircleTarget, Point, fRadius);

	D2D1_POINT_2F OtherPoint;
	float fOtherfRadius = 0.f;
	GetCircleValuesInRect(RectTarget, OtherPoint, fOtherfRadius);

	//피타고라스 정리 두원 센터 사이에 거리에따라 판별
	float a = Point.x - OtherPoint.x;
	float b = Point.y - OtherPoint.y;
	float c = fRadius + fOtherfRadius;
	if (a * a + b * b <= c *c)
		return TRUE;
	return FALSE;
}
//-----------------------------------------------------------------------------------------------------------
BOOL CColliderComponent::DifferentCollide(EntityPtr CircleTarget, EntityPtr RectTarget)
{
	if (!CircleColBetweenDifShape(CircleTarget, RectTarget))
		return FALSE;
	return true;
}
//-----------------------------------------------------------------------------------------------------------
void CColliderComponent::GetCircleValues(EntityPtr Target, D2D1_POINT_2F& Pos, float& fRadius) const
{
	Pos = Target->GetAttr<D2D1_POINT_2F>("Pos");
	float fWidth = Target->GetAttr<float>("Width");
	fRadius = fWidth * 0.5f;
}

void CColliderComponent::GetCircleValuesInRect(EntityPtr Target, D2D1_POINT_2F& Pos, float& fRadius) const
{
	Pos = Target->GetAttr<D2D1_POINT_2F>("Pos");
	float fHalfWidth = Target->GetAttr<float>("Width") * 0.5f;
	float fHalfHeight = Target->GetAttr<float>("Height")* 0.5f;
	fRadius = sqrtf(fHalfWidth * fHalfWidth + fHalfHeight  * fHalfHeight);
}
//-----------------------------------------------------------------------------------------------------------
bool CColliderComponent::OBBCol(EntityPtr otherTarget)
{
	D2D1_MATRIX_3X2_F OthermatTransform = otherTarget->GetAttr<D2D1_MATRIX_3X2_F>("Transform");
	D2D1_MATRIX_3X2_F matTransform = m_pEntity->GetAttr<D2D1_MATRIX_3X2_F>("Transform");
	D2D1_GEOMETRY_RELATION result = D2D1_GEOMETRY_RELATION_UNKNOWN;
	ID2D1Geometry* pTargetGeometry = m_pEntity->GetAttr<ID2D1Geometry*>("Geometry");
	ID2D1Geometry* pOtherTargetGeometry = otherTarget->GetAttr<ID2D1Geometry*>("Geometry");
	D2D1InvertMatrix(&matTransform);
	pTargetGeometry->CompareWithGeometry(pOtherTargetGeometry, OthermatTransform * matTransform, 0.1f, &result);
	if (result == D2D1_GEOMETRY_RELATION_OVERLAP || result == D2D1_GEOMETRY_RELATION_CONTAINS || result == D2D1_GEOMETRY_RELATION_IS_CONTAINED)
		return true;
	return false;
}
