#pragma once

#include "Entity.h"

class CRenderComponent : public CComponentBase
{
public:
	explicit CRenderComponent(EntityPtr pEntity, const dynamic& values);
	virtual ~CRenderComponent() {
		m_pBrush->Release();
		m_pBrush = nullptr;
		m_pGeometry->Release();
		m_pGeometry = nullptr;
	}

	DEFINE_DO_ATTR(CRenderComponent)

private:
	GET_ATTR
	SET_ATTR
	DO_ACTION

	virtual COMPONENT_ID GetComponentID() const override { return COMPONENT_ID::COM_RENDER; }
	
private:
	virtual void Render() override;

private:
	void OnSetColor(D2D1::ColorF::Enum eColor)
	{
		m_iColor = eColor;
		m_pBrush->SetColor(D2D1::ColorF(eColor, m_fAlpha));
	}
	void OnGetColor(D2D1::ColorF::Enum& eColor) const { eColor = static_cast<D2D1::ColorF::Enum>(m_iColor); }
	void OnSetColorInt(int iColor)
	{
		m_iColor = iColor;
		m_pBrush->SetColor(D2D1::ColorF(iColor, m_fAlpha));
	}
	void OnGetColorInt(int& iColor) const { iColor = m_iColor; }
	void OnSetAlpha(float fAlpha)
	{
		m_fAlpha = fAlpha;
		m_pBrush->SetColor(D2D1::ColorF(m_iColor, fAlpha));
	}
	void OnGetAlpha(float& fAlpha) const { fAlpha = m_fAlpha; }
	void OnSetHeightGeo(float fHeight)
	{
		m_pGeometry->Release();
		m_pGeometry = nullptr;

		m_fHeight = fHeight;

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
	void OnGetHeightGeo(float& fHeight) const { fHeight = m_fHeight; }
	bool DoBlink(const boost::any& param)
	{
		m_bColorBlink = boost::any_cast<bool>(param);
		if (!m_bColorBlink)
		{
			m_pBrush->SetColor(D2D1::ColorF(m_iColor, m_fAlpha));
		}
		return true;
	}
	bool DoDieColor(const boost::any& param)
	{
		m_pBrush->SetColor(D2D1::ColorF(boost::any_cast<int>(param), m_fAlpha));
		return true;
	}
	bool DoInvincible(const boost::any& param)
	{
		if (boost::any_cast<bool>(param))
			OnSetAlpha(0.5f);
		else
			OnSetAlpha(1.f);
		return true;
	}
private:
	int m_iColor;
	float m_fAlpha;
	RENDER_VALUE m_eValue;
	ID2D1SolidColorBrush* m_pBrush;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	bool m_bColorBlink;
	ID2D1Factory* m_pFactory;
	static unsigned m_iColorIndex;
	float m_fWidth;
	float m_fHeight;
	ID2D1Geometry* m_pGeometry;
};