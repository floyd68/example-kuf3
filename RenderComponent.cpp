#include "stdafx.h"
#include "RenderComponent.h"
#include "RenderMgr.h"
#include "KUF3Math.h"

unsigned CRenderComponent::m_iColorIndex = 0;
//-----------------------------------------------------------------------------------------------------------
BEGIN_DO_ATTR(CRenderComponent)

ATT_ITEM("Shape", RENDER_VALUE, m_eValue)
ATT_ITEM("RenderTarget", ID2D1HwndRenderTarget*, m_pRenderTarget)
ATT_ITEM("Geometry", ID2D1Geometry*, m_pGeometry)
ATT_ITEM("Width", float, m_fWidth)
ATT_ITEM("Height", float, m_fHeight)

CUSTOM_ATT_ITEM("Color", D2D1::ColorF::Enum, OnSetColor, OnGetColor)
CUSTOM_ATT_ITEM("Color_Int", int, OnSetColorInt, OnGetColorInt)
CUSTOM_ATT_ITEM("Alpha", float, OnSetAlpha, OnGetAlpha)
CUSTOM_ATT_ITEM("HeightGeo", float, OnSetHeightGeo, OnGetHeightGeo)

DO_ITEM("Invincibility", DoInvincible)
DO_ITEM("Blink", DoBlink)
DO_ITEM("DieColor", DoDieColor)

END_DO_ATTR
//-----------------------------------------------------------------------------------------------------------
CRenderComponent::CRenderComponent(EntityPtr pEntity, const dynamic& values)
	: CComponentBase(pEntity)
	, m_iColor(boost::any_cast<int>(values.at("Color")))
	, m_fAlpha(boost::any_cast<float>(values.at("Alpha")))
	, m_eValue(boost::any_cast<RENDER_VALUE>(values.at("Shape")))
	, m_pBrush(CRenderMgr::getSingleton()->CreateSolidBrush(m_iColor))
	, m_pRenderTarget(CRenderMgr::getSingleton()->getRenderTarget())
	, m_bColorBlink(false)
	, m_pFactory(CRenderMgr::getSingleton()->GetFactory())
	, m_fWidth(boost::any_cast<float>(values.at("Width")))
	, m_fHeight(boost::any_cast<float>(values.at("Height")))
{
	HRESULT hr = S_OK;
	switch (m_eValue)
	{
	case RENDER_VALUE::E_CIRCLE:
	case RENDER_VALUE::E_CIRCLE_N:
		ID2D1EllipseGeometry* pElapsedGeometry;
		hr = m_pFactory->CreateEllipseGeometry(D2D1::Ellipse(D2D1::Point2F(0, 0), m_fWidth * 0.5f, m_fHeight * 0.5f), &pElapsedGeometry);
		pElapsedGeometry->QueryInterface(IID_ID2D1Geometry, (VOID **)&m_pGeometry);
		pElapsedGeometry->Release();
		break;
	case RENDER_VALUE::E_RECT:
	case RENDER_VALUE::E_RECT_N:
		ID2D1RectangleGeometry* pRectangleGeometry;
		hr = m_pFactory->CreateRectangleGeometry(D2D1::RectF(-m_fWidth * 0.5f, -m_fHeight * 0.5f, m_fWidth * 0.5f, m_fHeight * 0.5f), &pRectangleGeometry);
		pRectangleGeometry->QueryInterface(IID_ID2D1Geometry, (VOID **)&m_pGeometry);
		pRectangleGeometry->Release();
		break;
	}
}
//-----------------------------------------------------------------------------------------------------------
void CRenderComponent::Render()
{
	D2D1_MATRIX_3X2_F matWorld = m_pEntity->GetAttr<D2D1_MATRIX_3X2_F>("Transform");
	m_pRenderTarget->SetTransform(matWorld);
	switch (m_eValue)
	{
	case RENDER_VALUE::E_RECT:
	case RENDER_VALUE::E_CIRCLE:
		m_pRenderTarget->FillGeometry(m_pGeometry, m_pBrush);
		break;

	case RENDER_VALUE::E_RECT_N:
	case RENDER_VALUE::E_CIRCLE_N:
		m_pRenderTarget->DrawGeometry(m_pGeometry, m_pBrush, 1);
		break;
	}

	if (m_bColorBlink)
	{
		m_iColorIndex = rand() % 3;

		static D2D1::ColorF::Enum s_ColorBuf[] = {
			D2D1::ColorF::Red,
			D2D1::ColorF::Orange,
			D2D1::ColorF::Yellow
		};

		int iColor = s_ColorBuf[m_iColorIndex];
		++m_iColorIndex;
		m_pBrush->SetColor(D2D1::ColorF(iColor, m_fAlpha));
	}
}
