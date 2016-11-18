#pragma once

class CInputMgr;
#include "SingletonBase.h"

class Session;

class CMyMain : public CSingletonBase<CMyMain>
{
public:
	CMyMain();
	~CMyMain();

public:
	typedef std::shared_ptr<CInputMgr> InputMgrPtr;
	typedef std::shared_ptr<Session> SessionPtr;
	InputMgrPtr GetInputMgr() const { return m_pInputMgr; }

public:
	void Initialize();
	void Update();
	void Render();
	void Release();

	SessionPtr GetSessionPtr(){ return m_pSession; }

private:
	void ShowFPS();

private:
	unsigned m_iFPS;
	TCHAR m_szFPS[128];
	float m_fFPSTime;

	InputMgrPtr m_pInputMgr;
	SessionPtr m_pSession;
};