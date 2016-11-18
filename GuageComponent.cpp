#include "stdafx.h"
#include "GuageComponent.h"
#include "KUF3Value.h"
#include "RenderMgr.h"
//-----------------------------------------------------------------------------------------------------------
BEGIN_DO_ATTR(CGuageComponent)

CUSTOM_ATT_ITEM("BackGuageColor", D2D1::ColorF::Enum, OnSetBackColor, OnGetBackColor)
CUSTOM_ATT_ITEM("FrontGuageColor", D2D1::ColorF::Enum, OnSetFrontColor, OnGetFrontColor)
END_DO_ATTR
//-----------------------------------------------------------------------------------------------------------
CGuageComponent::CGuageComponent(EntityPtr pEntity, const dynamic& values) : CComponentBase(pEntity)
,m_pFactory(CRenderMgr::getSingleton()->GetFactory())
, m_iBackGuageColor(boost::any_cast<int>(values.at("BackGuageColor")))
, m_iFronGuageColor(boost::any_cast<int>(values.at("FrontGuageColor")))
, m_pBackBrush(CRenderMgr::getSingleton()->CreateSolidBrush(m_iBackGuageColor))
, m_pFrontBrush(CRenderMgr::getSingleton()->CreateSolidBrush(m_iFronGuageColor))
{
	HRESULT hr;
	hr = m_pFactory->CreateRectangleGeometry(D2D1::RectF(-GUAGE_WIDTH * 0.5f, -GUAGE_HEIGHT * 0.5f, GUAGE_WIDTH * 0.5f, GUAGE_HEIGHT * 0.5f), &m_pRectangleGeometry);
}
//-----------------------------------------------------------------------------------------------------------
void CGuageComponent::Render()
{
	float fHp = m_pEntity->GetAttr<float>("Hp");
	float fCurHp = m_pEntity->GetAttr<float>("CurHp");

	if (fCurHp == fHp)
		return;

	D2D1_POINT_2F Point = m_pEntity->GetAttr<D2D1_POINT_2F>("Pos");
	float fHeight = m_pEntity->GetAttr<float>("Height");
	ID2D1HwndRenderTarget* pRenderTarget = m_pEntity->GetAttr<ID2D1HwndRenderTarget*>("RenderTarget");

	D2D1_MATRIX_3X2_F matScale = D2D1::Matrix3x2F::Scale(D2D1::SizeF(1.f, 1.f));
	D2D1_MATRIX_3X2_F matTrans = D2D1::Matrix3x2F::Translation(Point.x, Point.y - (fHeight * 0.5f) - 20.f);
	D2D1_MATRIX_3X2_F matWorld = matScale * matTrans;
	pRenderTarget->SetTransform(matWorld);

	pRenderTarget->FillGeometry(m_pRectangleGeometry, m_pBackBrush);

	matScale = D2D1::Matrix3x2F::Scale(D2D1::SizeF(fCurHp / fHp, 1.f));
	matTrans = D2D1::Matrix3x2F::Translation(Point.x - ((GUAGE_WIDTH - (GUAGE_WIDTH * fCurHp / fHp)) * 0.5f), Point.y - (fHeight * 0.5f) - 20.f);
	matWorld = matScale * matTrans;
	pRenderTarget->SetTransform(matWorld);

	pRenderTarget->FillGeometry(m_pRectangleGeometry, m_pFrontBrush);
}