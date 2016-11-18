#include "stdafx.h"
#include "AttackBoxComponent.h"

//-----------------------------------------------------------------------------------------------------------
void CAttackBoxComponent::Update(float fElapsedTime)
{
	if (m_fAlpha < 0.f)
		m_pEntity->SetIsDelete();

	m_fAlpha = m_fAlpha - fElapsedTime * 1.5f;

	m_pEntity->SetAttr("Alpha", m_fAlpha);
}