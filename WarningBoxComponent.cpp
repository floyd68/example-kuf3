#include "stdafx.h"
#include "WarningBoxComponent.h"
#include "KUF3Struct.h"

//-----------------------------------------------------------------------------------------------------------
void CWarningBoxComponent::Update(float fElapsedTime)
{
	m_fDeleteTime += fElapsedTime;

	if (m_fDeleteTime > 1.f)
	{
		AttackOption tAttackOption;
		D2D1_POINT_2F Point = m_pEntity->GetAttr<D2D1_POINT_2F>("Pos");
		tAttackOption.fPosX = Point.x;
		tAttackOption.fPosY = Point.y;
		tAttackOption.fWidth = m_pEntity->GetAttr<float>("Width");
		tAttackOption.fHeight = m_pEntity->GetAttr<float>("Height");
		tAttackOption.DirPt = m_pEntity->GetAttr<D2D1_POINT_2F>("Dir");
		tAttackOption.matScale = m_pEntity->GetAttr<D2D1_MATRIX_3X2_F>("Scale");

		m_pEntity->Do("CreateAttackBox", tAttackOption);

		m_pEntity->SetIsDelete();
	}
}