#pragma once

#include "SingletonBase.h"

class CRenderMgr : public CSingletonBase <CRenderMgr>
{
public:
	CRenderMgr();
	~CRenderMgr() {}

public:
	void Initialize();
	void Render();
	void Release();

public:
	ID2D1HwndRenderTarget* getRenderTarget() const { return m_pRenderTarget; }
	ID2D1Factory* GetFactory()const { return m_pFactory; }
	ID2D1SolidColorBrush* CreateSolidBrush(int color) const
	{
		ID2D1SolidColorBrush* pBrush = nullptr;
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color), &pBrush);
		return pBrush;
	}

private:
	void RenderText();

private:
	ID2D1Factory* m_pFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	IDWriteFactory* m_pWriteFactory;
	IDWriteTextFormat* m_pStageTextFormat;
	IDWriteTextFormat* m_pScoreTextFormat;
	ID2D1SolidColorBrush* m_pBrush;

};