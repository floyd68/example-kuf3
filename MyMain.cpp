#include "stdafx.h"
#include "MyMain.h"
#include "KUF3Value.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "GameLogicMgr.h"
#include "RenderMgr.h"
#include "Session.h"


//-----------------------------------------------------------------------------------------------------------
CMyMain::CMyMain()
	: m_iFPS(0)
	, m_fFPSTime(0.f)
	, m_pInputMgr(new CInputMgr)
{
	CGameLogicMgr::getSingleton();
	CRenderMgr::getSingleton();
	CTimeMgr::getSingleton();

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	m_pSession = std::make_shared<Session>();
}
//-----------------------------------------------------------------------------------------------------------
CMyMain::~CMyMain()
{
	CGameLogicMgr::getSingleton()->releaseSingleton();
	CRenderMgr::getSingleton()->releaseSingleton();
	CTimeMgr::getSingleton()->releaseSingleton();;

	WSACleanup();
}
//-----------------------------------------------------------------------------------------------------------
void CMyMain::Initialize()
{
	srand(GetTickCount());

	m_pSession->Connect("127.0.0.1", 9999);

	CRenderMgr::getSingleton()->Initialize();
	CGameLogicMgr::getSingleton()->Initialize();
	CTimeMgr::getSingleton()->Initialize();
}
//-----------------------------------------------------------------------------------------------------------
void CMyMain::Update()
{
	CTimeMgr::getSingleton()->Update();
	CGameLogicMgr::getSingleton()->Update(CTimeMgr::getSingleton()->GetTime());
	m_pSession->Update();

	/*char buf[512];
	strcpy(buf, "abc");
	m_pSession->Send(&buf, 4);*/
}
//-----------------------------------------------------------------------------------------------------------
void CMyMain::Render()
{
	CRenderMgr::getSingleton()->Render();
	ShowFPS();
}
//-----------------------------------------------------------------------------------------------------------
void CMyMain::Release()
{
	CRenderMgr::getSingleton()->Release();
}
//-----------------------------------------------------------------------------------------------------------
void CMyMain::ShowFPS()
{
	++m_iFPS;

	m_fFPSTime += CTimeMgr::getSingleton()->GetTime();

	if (m_fFPSTime > 1.f)
	{
		wsprintf(m_szFPS, L"FPS : %d", m_iFPS);
		m_iFPS = 0;
		m_fFPSTime = 0.f;
	}

	SetWindowText(g_hWnd, m_szFPS);
}
