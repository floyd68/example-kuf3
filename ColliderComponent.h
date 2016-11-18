#pragma once

#include "Entity.h"



class CColliderComponent : public CComponentBase
{
public:
	explicit CColliderComponent(EntityPtr pEntity, const dynamic& values);
	virtual ~CColliderComponent() {}

	DEFINE_DO_ATTR(CColliderComponent)

public:
	GET_ATTR
	SET_ATTR
	DO_ACTION

	virtual COMPONENT_ID GetComponentID() const { return COMPONENT_ID::COM_COLLIDER; }

public:
	virtual void Update(float fElapsedTime);

private:
	bool IsCollided(EntityPtr otherTarget);
	bool IsInCollided(EntityPtr otherTarget);
	bool OBBCol(EntityPtr otherTarget);
	BOOL CollideEntity(EntityPtr otherTarget);
	BOOL RectCollide(EntityPtr otherTarget);
	BOOL CircleCollide(EntityPtr otherTarget);
	bool CircleColBetweenDifShape(EntityPtr CircleTarget, EntityPtr RectTarget);
	BOOL DifferentCollide(EntityPtr CircleTarget, EntityPtr RectTarget);
	void GetCircleValues(EntityPtr Target, D2D1_POINT_2F& Pos, float& fRadius) const;
	void GetCircleValuesInRect(EntityPtr Target, D2D1_POINT_2F& Pos, float& fRadius) const;
private:
	typedef std::vector<EntityWeakPtr> vCol;
	vCol m_vecCol;
	OBJ_TYPE m_ColObjType;

};