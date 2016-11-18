#pragma once
#include "SingletonBase.h"

class CInputMgr 
{
public:
	CInputMgr() {}
	~CInputMgr() {}

public:
	void CheckMouse();
	void OnButtonDown(int param);
	void OnRButtonDown(float fPosX, float fPosY);
};