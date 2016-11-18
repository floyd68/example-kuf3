#pragma once

#include "SingletonBase.h"
#include "ComponentBase.h"
#include "SMsg.h"

class CEntity;
enum class ENTITY_TYPE;

class CGameLogicMgr : public CSingletonBase <CGameLogicMgr>
{
public:
	CGameLogicMgr()
		: m_bIsHost(false)
		, m_bIsAllClear(false)
		, m_bIsDied(false)
		, m_iStageLv(0)
		, m_iWave(0)
		, m_iScore(0)
		, m_fWaveTime(0.f)
		, m_pPlayer(nullptr)
		{}
	~CGameLogicMgr();

public:
	void Initialize();
	void Update(float fElapsedTime);

public:
	unsigned GetStageLevel() const { return m_iStageLv; }
	unsigned GetScore() const { return m_iScore; }
	bool GetIsAllClear() const { return m_bIsAllClear; }
	bool GetIsDied() const { return m_bIsDied; }
	std::string* GetRadomItemTable(){ return s_RandomItem; }
	void ScorePlus(unsigned iScore) { m_iScore += iScore; }
	void SetDied(bool bIsDied);
	void SetStageClear();
	void SetHost()
	{
		m_bIsHost = true;
		CreateEnemy();
		++m_iWave;
	}
	EntityPtr CreateObject(const std::string& strName, D2D1_POINT_2F Point = { 0.f, 0.f });
	void CreateObjectMsg(const std::string& strName, D2D1_POINT_2F Point = { 0.f, 0.f });

private:
	bool EnemyEmptyCheck();
	void CreateEnemy();
	void StageWave(float fElapsedTime);

	bool CreateComStruct(const char* szName);
	bool LoadStageEnemyInfo();
	void CreateStageEnemyInfo(const rapidjson::Value& value);

private:
	bool m_bIsHost;
	bool m_bIsAllClear;
	bool m_bIsDied;
	unsigned m_iStageLv;
	unsigned m_iWave;
	unsigned m_iScore;
	float m_fWaveTime;

	EntityPtr m_pPlayer;

	static std::string s_RandomItem[];

	struct EnemyCreateInfo
	{
		std::string strEnemyType;
		D2D1_POINT_2F tPos;
	};

	typedef std::vector<std::vector<std::vector<EnemyCreateInfo>>> ENEMY_WAVE_INFO;
	ENEMY_WAVE_INFO m_vecEnemyWaveInfo;
};