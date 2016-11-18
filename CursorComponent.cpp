#include "stdafx.h"
#include "CursorComponent.h"

//-----------------------------------------------------------------------------------------------------------
void CCursorComponent::Update(float fElapsedTime)
{
	if (m_fScale < 0.2f)
		m_pEntity->SetIsDelete();

	m_fScale = m_fScale - fElapsedTime * 1.5f;

	D2D1_MATRIX_3X2_F matScale = D2D1::Matrix3x2F::Scale(m_fScale, m_fScale);
	m_pEntity->SetAttr("Scale", matScale);
}