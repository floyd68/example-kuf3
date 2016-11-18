#pragma once

#include "Entity.h"

class CInputComponent;

class CPositionComponent : public CComponentBase
{
public:
	explicit CPositionComponent(EntityPtr pEntity, const dynamic& values);
	virtual ~CPositionComponent() 	{}

	DEFINE_DO_ATTR(CPositionComponent)

private:
	GET_ATTR
	SET_ATTR
	DO_ACTION

	virtual COMPONENT_ID GetComponentID() const { return COMPONENT_ID::COM_POS; }

private:
	virtual void Update(float fElapsedTime);

private:
	bool DoMove(const boost::any& param);
	bool DoRapierMove(const boost::any& param);
	bool DoStopMove(const boost::any& param);
	bool DoRButtonDown(const boost::any& param);
	bool DoMovePacket(const boost::any& param);
	void MoveToDir(const boost::any& param);
	bool DoSendPosObjInfo(const boost::any& param);
	bool DoGetSpeedItem(const boost::any& param)
	{
		switch (boost::any_cast<ITEM_ID>(param))
		{
		case ITEM_ID::ITEM_SPEEDUP:
			m_pEntity->Do("Blink", true);
			m_bSpeedUp = true;
			m_fSpeed += 20.f;
			return true;
		default:
			break;
		}
		return false;
	}
	bool DoResetSpeed(const boost::any& param)
	{
		if (m_bSpeedUp)
		{
			m_pEntity->Do("Blink", false);
			m_bSpeedUp = false;
			m_fSpeedUpTime = 0.f;
			m_fSpeed -= 20.f;
		}
		return true;
	}
	bool DoTargetPos(const boost::any& param)
	{
		m_StartPoint = m_Point;
		m_TargetPoint = boost::any_cast<D2D1_POINT_2F>(param);
		float a = m_TargetPoint.x - m_StartPoint.x;
		float b = m_TargetPoint.y - m_StartPoint.y;
		float c = sqrt(a * a + b * b);
		//방향벡터를 위해 정규화
		m_Speed.x = a / c;
		m_Speed.y = b / c;
		m_DirPoint = m_Speed;
		return true;
	}
private:
	bool CheckOutScreen();

private:
	bool m_bSpeedUp;
	UNIT_DIRECTION m_eDirection;
	float m_fSpeed;
	float m_fSpeedUpTime;
	bool m_bIsPossibleScreen;
	D2D1_POINT_2F m_Point;
	D2D1_POINT_2F m_TargetPoint;
	D2D1_POINT_2F m_StartPoint;
	D2D1_MATRIX_3X2_F m_matScale;
	D2D1_MATRIX_3X2_F m_matWorld;
	bool m_bIsTargetPt;
	D2D1_POINT_2F m_Speed;
	D2D1_POINT_2F m_DirPoint;
};