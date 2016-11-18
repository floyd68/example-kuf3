#pragma once

#include "SingletonBase.h"

class CTimeMgr : public CSingletonBase <CTimeMgr>
{
public:
	CTimeMgr() : m_fTime(0.f) {}
	~CTimeMgr() {}

public:
	float GetTime() const { return m_fTime; }

public:
	void Initialize()
	{
		QueryPerformanceCounter(&m_FrameTime);
		QueryPerformanceCounter(&m_FixTime);
		QueryPerformanceCounter(&m_LastTime);
		QueryPerformanceFrequency(&m_CpuTick);
	}
	void Update()
	{
		QueryPerformanceCounter(&m_FrameTime);

		if (m_FrameTime.QuadPart - m_LastTime.QuadPart > m_CpuTick.QuadPart)
		{
			QueryPerformanceFrequency(&m_CpuTick);
			m_LastTime.QuadPart = m_FrameTime.QuadPart;
		}

		m_fTime = static_cast<float>(m_FrameTime.QuadPart - m_FixTime.QuadPart) / static_cast<float>(m_CpuTick.QuadPart);
		m_FixTime = m_FrameTime;
	}

private:
	float m_fTime;
	LARGE_INTEGER m_FrameTime;
	LARGE_INTEGER m_FixTime;
	LARGE_INTEGER m_LastTime;
	LARGE_INTEGER m_CpuTick;
};