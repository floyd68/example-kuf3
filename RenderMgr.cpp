#include "stdafx.h"
#include "RenderMgr.h"
#include "KUF3Value.h"
#include "ObjectMgr.h"
#include "GameLogicMgr.h"

//-----------------------------------------------------------------------------------------------------------
CRenderMgr::CRenderMgr()
	: m_pFactory(nullptr)
	, m_pRenderTarget(nullptr)
	, m_pWriteFactory(nullptr)
	, m_pStageTextFormat(nullptr)
	, m_pScoreTextFormat(nullptr)
	, m_pBrush(nullptr) {}
//-----------------------------------------------------------------------------------------------------------
void CRenderMgr::Initialize()
{
	HRESULT hr;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory);
	hr = m_pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(g_hWnd, D2D1::SizeU(WIN_SIZE_X, WIN_SIZE_Y), D2D1_PRESENT_OPTIONS_IMMEDIATELY), &m_pRenderTarget);
	m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pWriteFactory));
	hr = m_pWriteFactory->CreateTextFormat(TEXT("Consolas"), nullptr, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 40.f, TEXT("en-us"), &m_pStageTextFormat);
	hr = m_pWriteFactory->CreateTextFormat(TEXT("Consolas"), nullptr, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20.f, TEXT("en-us"), &m_pScoreTextFormat);

	m_pBrush = CreateSolidBrush(D2D1::ColorF::Black);
}
//-----------------------------------------------------------------------------------------------------------
void CRenderMgr::Render()
{
	m_pRenderTarget->BeginDraw();
	m_pRenderTarget->Clear(D2D1::ColorF(1.f, 1.f, 1.f));

	CObjectMgr::getSingleton()->Render();

	RenderText();

	m_pRenderTarget->EndDraw();
}
//-----------------------------------------------------------------------------------------------------------
void CRenderMgr::Release()
{
	m_pFactory->Release();
	m_pFactory = nullptr;
	m_pRenderTarget->Release();
	m_pRenderTarget = nullptr;

	m_pStageTextFormat->Release();
	m_pStageTextFormat = nullptr;
	m_pScoreTextFormat->Release();
	m_pScoreTextFormat = nullptr;
	m_pWriteFactory->Release();
	m_pWriteFactory = nullptr;
	m_pBrush->Release();
	m_pBrush = nullptr;
}
//-----------------------------------------------------------------------------------------------------------
void CRenderMgr::RenderText()
{
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	wchar_t buf[16];

	swprintf_s(buf, L"Score : %d", CGameLogicMgr::getSingleton()->GetScore());
	m_pRenderTarget->DrawText(buf, wcslen(buf), m_pScoreTextFormat, D2D1::RectF(0, 0, 300, 50), m_pBrush);

	if (CGameLogicMgr::getSingleton()->GetIsAllClear())
	{
		wcscpy_s(buf, L"Stage Clear !");
		m_pRenderTarget->DrawText(buf, wcslen(buf), m_pStageTextFormat, D2D1::RectF(WIN_SIZE_X << 1, WIN_SIZE_Y << 1, 300, 300), m_pBrush);
	}
	else
	{
		swprintf_s(buf, L"Stage : %d", CGameLogicMgr::getSingleton()->GetStageLevel() + 1);
		m_pRenderTarget->DrawText(buf, wcslen(buf), m_pStageTextFormat, D2D1::RectF(WIN_SIZE_X << 1, 0, 300, 50), m_pBrush);
	}

	if (CGameLogicMgr::getSingleton()->GetIsDied())
	{
		wcscpy_s(buf, L"You Died!");
		m_pRenderTarget->DrawText(buf, wcslen(buf), m_pStageTextFormat, D2D1::RectF(WIN_SIZE_X << 1, WIN_SIZE_Y << 1, 300, 300), m_pBrush);

		wcscpy_s(buf, L"Press Enter");
		m_pRenderTarget->DrawText(buf, wcslen(buf), m_pStageTextFormat, D2D1::RectF(WIN_SIZE_X << 1, WIN_SIZE_Y << 1, 280, 350), m_pBrush);
	}
}