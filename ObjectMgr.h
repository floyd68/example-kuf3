#pragma once

#include "SingletonBase.h"
#include "ComponentBase.h"

class CEntity;

class CObjectMgr : public CSingletonBase <CObjectMgr>
{
public:
	CObjectMgr() {}

public:
	void Update(float fElapsedTime);
	void Render();

public:
	bool Do(const std::string& strAction, const boost::any& param = 0);
	template <typename T> void for_each_entities(const T& pred)
	{
		for (auto entity : m_vecObject)
			pred(entity);
	}
	template <typename T> void for_each_Addentities(const T& pred)
	{
		for (auto entity : m_vecAddObject)
			pred(entity);
	}
	template <typename T> EntityPtr find_for(T pred)
	{
		for (auto entity : m_vecObject)
		{
			if (pred(entity))
				return entity;
		}
		return nullptr;
	}
	EntityPtr GetLastEntity(int Index)
	{
		if (m_vecAddObject.empty())
		return m_vecObject[Index];
	}
	size_t GetvecObjSize(){ return m_vecObject.size(); }
	size_t GetvecAddObjSize(){ return m_vecAddObject.size(); }
public:
	void AddObject(EntityPtr pEntity)
	{
		m_vecAddObject.push_back(pEntity);
	}
	// std::vector<CEntity*> GetVecObj(){ return m_vecObject; }
	// vector를 by reference도 아닌 by value로 넘기는 것은 최악의 퍼포먼스를 보장함.
	// 구조적으로 볼때도 Manager 내의 개체를 그대로 외부로 넘겨주는 것은 절대로 피해야함.
	// 대신 위의 for_each_entities, find_for 같은 함수를 사용하거나 비슷한 함수를 만들어 사용하여야 함.

private:
	std::vector< EntityPtr > m_vecObject;
	std::vector< EntityPtr > m_vecAddObject;
};