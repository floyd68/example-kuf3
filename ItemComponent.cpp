#include "stdafx.h"
#include "ItemComponent.h"

//-----------------------------------------------------------------------------------------------------------
BEGIN_DO_ATTR(CItemComponent)

ATT_ITEM("Item", ITEM_ID, m_eItemID)

END_DO_ATTR

void CItemComponent::Update(float fElapsedTime)
{
	m_fItemDisappearTime += fElapsedTime;
	if (m_fItemDisappearTime > 2.f)
		m_pEntity->SetIsDelete();
}