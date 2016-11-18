#pragma once

#include "Entity.h"

class CGuageComponent : public CComponentBase
{
public:
	CGuageComponent(EntityPtr pEntity, const dynamic& values);
	virtual ~CGuageComponent() {}

	DEFINE_DO_ATTR(CGuageComponent)
private:
	GET_ATTR
	SET_ATTR

public:
	virtual COMPONENT_ID GetComponentID() const override { return COMPONENT_ID::COM_GUAGE; }

public:
	virtual void Render() override;
private:
	void OnSetBackColor(D2D1::ColorF::Enum eColor)
	{
		m_iBackGuageColor = eColor;
		m_pBackBrush->SetColor(D2D1::ColorF(eColor, 1.f));
	}
	void OnGetBackColor(D2D1::ColorF::Enum& eColor) const { eColor = static_cast<D2D1::ColorF::Enum>(m_iBackGuageColor); }
	void OnSetFrontColor(D2D1::ColorF::Enum eColor)
	{
		m_iFronGuageColor = eColor;
		m_pFrontBrush->SetColor(D2D1::ColorF(eColor, 1.f));
	}
	void OnGetFrontColor(D2D1::ColorF::Enum& eColor) const { eColor = static_cast<D2D1::ColorF::Enum>(m_iFronGuageColor); }
private:
	int m_iBackGuageColor;
	int m_iFronGuageColor;
	ID2D1RectangleGeometry* m_pRectangleGeometry;
	ID2D1Factory* m_pFactory;
	ID2D1SolidColorBrush* m_pBackBrush;
	ID2D1SolidColorBrush* m_pFrontBrush;
};