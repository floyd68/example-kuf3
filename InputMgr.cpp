#include "stdafx.h"
#include "InputMgr.h"
#include "KUF3Value.h"
#include "ObjectMgr.h"

//-----------------------------------------------------------------------------------------------------------
void CInputMgr::CheckMouse()
{
	RECT rcWindow;
	GetWindowRect(g_hWnd, &rcWindow);

	RECT rcLp = {0, 0, 0, 0};
	AdjustWindowRect(&rcLp, WS_OVERLAPPEDWINDOW, FALSE);

	rcWindow.bottom -= rcLp.bottom;
	rcWindow.left -= rcLp.left;
	rcWindow.right -= rcLp.right;
	rcWindow.top -= rcLp.top;

	ClipCursor(&rcWindow);
}
//-----------------------------------------------------------------------------------------------------------
void CInputMgr::OnButtonDown(int param)
{
	CObjectMgr::getSingleton()->Do("InputDown", param);
}
//-----------------------------------------------------------------------------------------------------------
void CInputMgr::OnRButtonDown(float fPosX, float fPosY)
{
	CObjectMgr::getSingleton()->Do("InputRbtDown", D2D1_POINT_2F{ fPosX, fPosY });
}