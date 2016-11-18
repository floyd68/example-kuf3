#include "stdafx.h"
#include "ObjectMgr.h"
#include "Entity.h"

//-----------------------------------------------------------------------------------------------------------
void CObjectMgr::Update(float fElapsedTime)
{
	auto iter = m_vecObject.begin();
	for (; iter < m_vecObject.end();)
	{
		if (!(*iter)->IsDelete())
			++iter;
		else
			iter = m_vecObject.erase(iter);
	}

	m_vecObject.insert(m_vecObject.end(), m_vecAddObject.begin(), m_vecAddObject.end());
	m_vecAddObject.clear();

	for (auto Obj : m_vecObject)
	{
		Obj->Update(fElapsedTime);
	}
}
//-----------------------------------------------------------------------------------------------------------
void CObjectMgr::Render()
{
	for (auto pEntity : m_vecObject)
	{
		pEntity->Render();
	}
}
//-----------------------------------------------------------------------------------------------------------
bool CObjectMgr::Do(const std::string& strAction, const boost::any& param)
{
	for (auto Obj : m_vecObject)
	{
		Obj->Do(strAction, param);
	}
	return true;
}