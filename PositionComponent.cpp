#include "stdafx.h"
#include "PositionComponent.h"
#include "KUF3Value.h"
#include "TimeMgr.h"
#include "InputComponent.h"
#include "RenderMgr.h"
#include "KUF3Math.h"
#include "MyMain.h"
#include "SMsg.h"
#include "Session.h"

//-----------------------------------------------------------------------------------------------------------
BEGIN_DO_ATTR(CPositionComponent)

ATT_ITEM("Pos", D2D1_POINT_2F, m_Point)
ATT_ITEM("Speed", float, m_fSpeed)
ATT_ITEM("OutScreen", bool, m_bIsPossibleScreen)
ATT_ITEM("Direction", UNIT_DIRECTION, m_eDirection)
ATT_ITEM("Dir", D2D1_POINT_2F, m_DirPoint)
ATT_ITEM("Transform", D2D1_MATRIX_3X2_F, m_matWorld)
ATT_ITEM("Scale", D2D1_MATRIX_3X2_F, m_matScale)

DO_ITEM("Move", DoMove)
DO_ITEM("StopMove", DoStopMove)
DO_ITEM("SpeedItem", DoGetSpeedItem)
DO_ITEM("RapierMove", DoRapierMove)
DO_ITEM("ResetSpeed", DoResetSpeed)
DO_ITEM("RBtDown", DoRButtonDown)
DO_ITEM("MovePacket", DoMovePacket)
DO_ITEM("SetTargetPos", DoTargetPos)
DO_ITEM("SendPosObjInfo", DoSendPosObjInfo)

END_DO_ATTR
//-----------------------------------------------------------------------------------------------------------
CPositionComponent::CPositionComponent(EntityPtr pEntity, const dynamic& values)
: CComponentBase(pEntity)
, m_bSpeedUp(false)
, m_eDirection(UNIT_DIRECTION::DIR_UP)
, m_fSpeed(0.f)
, m_fSpeedUpTime(0.f)
, m_bIsTargetPt(true)
, m_matScale(D2D1::IdentityMatrix())
, m_matWorld(D2D1::IdentityMatrix())
, m_bIsPossibleScreen(boost::any_cast<bool>(values.at("OutScreen")))
{
	m_Speed = D2D1_POINT_2F{ 0, 0 };
	m_DirPoint = D2D1_POINT_2F{ 0, -1 };
}


//-----------------------------------------------------------------------------------------------------------
void CPositionComponent::Update(float fElapsedTime)
{
	if (m_bSpeedUp)
	{
		m_fSpeedUpTime += fElapsedTime;
		if (m_fSpeedUpTime > 5.f)
		{
			m_pEntity->Do("Blink", false);
			m_bSpeedUp = false;
			m_fSpeedUpTime = 0.f;
			m_fSpeed -= 20.f;
		}
	}

	if (!m_bIsTargetPt)
	{
		if (abs(m_TargetPoint.x - m_StartPoint.x) < abs(m_Point.x - m_StartPoint.x))
		{
			m_Speed = D2D1_POINT_2F{ 0, 0 };
			m_Point = m_TargetPoint;
			m_bIsTargetPt = true;
		}
	}

	m_Point.x += m_Speed.x * m_fSpeed * CTimeMgr::getSingleton()->GetTime();
	m_Point.y += m_Speed.y * m_fSpeed * CTimeMgr::getSingleton()->GetTime();

	if (CheckOutScreen())
		m_pEntity->Do("OutOfScreen");

	D2D1_MATRIX_3X2_F matRot = D2D1::Matrix3x2F::Rotation(DirVecToDegree(m_DirPoint));
	D2D1_MATRIX_3X2_F matTrans = D2D1::Matrix3x2F::Translation(m_Point.x, m_Point.y);
	m_matWorld = m_matScale * matRot * matTrans;
}
//-----------------------------------------------------------------------------------------------------------
bool CPositionComponent::DoMove(const boost::any& param)
{
	static std::pair<float, float> s_MovePos[] = {
		{ -1.f, 0.f },
		{ 1.f, 0.f },
		{ 0.f, -1.f },
		{ 0.f, 1.f },
	};


	auto MovePos = s_MovePos[static_cast<int>(boost::any_cast<UNIT_DIRECTION>(param))];

	m_Point.x += MovePos.first * m_fSpeed * CTimeMgr::getSingleton()->GetTime();
	m_Point.y += MovePos.second * m_fSpeed * CTimeMgr::getSingleton()->GetTime();


	if (CheckOutScreen())
		m_pEntity->Do("OutOfScreen");

	return false;
}


bool CPositionComponent::DoRapierMove(const boost::any& param)
{
	m_Speed = D2D1_POINT_2F{ 0, 0 };
	m_bIsTargetPt = true;
	m_Point.x += m_DirPoint.x * 175.f;
	m_Point.y += m_DirPoint.y * 175.f;
	return false;

}
bool CPositionComponent::DoStopMove(const boost::any& param)
{
	m_Speed = boost::any_cast<D2D1_POINT_2F>(param);
	CheckOutScreen();
	return true;
}
//-----------------------------------------------------------------------------------------------------------
bool CPositionComponent::CheckOutScreen()
{
	if (m_bIsPossibleScreen)
		return false;
	bool bIsOut = false;
	float fHalfWidth = m_pEntity->GetAttr<float>("Width") * 0.5f;
	float fHalfHeight = m_pEntity->GetAttr<float>("Height") * 0.5f;
	if (m_Point.y < fHalfHeight)
	{
		m_Point.y = fHalfHeight;
		bIsOut = true;
	}
	if (m_Point.y > WIN_SIZE_Y - fHalfHeight)
	{
		m_Point.y = WIN_SIZE_Y - fHalfHeight;
		bIsOut = true;
	}
	if (m_Point.x < fHalfWidth)
	{
		m_Point.x = fHalfWidth;
		bIsOut = true;
	}
	if (m_Point.x > WIN_SIZE_X - fHalfWidth)
	{
		m_Point.x = WIN_SIZE_X - fHalfWidth;
		bIsOut = true;
	}
	return bIsOut;
}

bool CPositionComponent::DoRButtonDown(const boost::any& param)
{
	MoveToDir(param);
	SMsgSend msg(static_cast<unsigned short>(PACKET_ID::PACKET_MOVE));
	msg << m_pEntity->GetEntityID() << m_Point.x << m_Point.y << m_TargetPoint.x << m_TargetPoint.y;
	CMyMain::getSingleton()->GetSessionPtr()->Send(msg);

	return true;
}
bool CPositionComponent::DoMovePacket(const boost::any& param)
{
	MoveToDir(param);
	return true;
}
void  CPositionComponent::MoveToDir(const boost::any& param)
{
	m_StartPoint.x = m_Point.x;
	m_StartPoint.y = m_Point.y;
	m_TargetPoint = boost::any_cast<D2D1_POINT_2F>(param);
	m_bIsTargetPt = false;
	float a = m_TargetPoint.x - m_StartPoint.x;
	float b = m_TargetPoint.y - m_StartPoint.y;
	float c = sqrt(a * a + b * b);
	//방향벡터를 위해 정규화
	m_Speed.x = a / c;
	m_Speed.y = b / c;
	m_DirPoint = m_Speed;
}

bool CPositionComponent::DoSendPosObjInfo(const boost::any& param)
{
	SMsgSend msg(static_cast<unsigned short>(PACKET_ID::PACKET_SEND_POS_INFO));
	msg << m_pEntity->GetEntityID() << m_bSpeedUp << m_fSpeed << m_fSpeedUpTime;
	CMyMain::getSingleton()->GetSessionPtr()->Send(msg);
	return true;
}