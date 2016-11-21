#include "stdafx.h"
#include "GameLogicMgr.h"
#include "KUF3Value.h"
#include "KUF3Math.h"
#include "ObjectMgr.h"
#include "EntityFactory.h"
#include "Session.h"
#include "MyMain.h"

//-----------------------------------------------------------------------------------------------------------
std::string CGameLogicMgr::s_RandomItem[] = {
	"TriCellItem",
	"SpeedItem",
	"HpItem",
	"AttItem",
	"RapierItem",
	"ShotGunItem",
	"RifleItem",
};
//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
CGameLogicMgr::~CGameLogicMgr()
{
	CObjectMgr::getSingleton()->releaseSingleton();
}
//-----------------------------------------------------------------------------------------------------------
void CGameLogicMgr::Initialize()
{
	std::vector<const char*> vecEntityInfoFileName = {
		"Player",
		"OtherPlayer",
		"Bat",
		"Skeleton",
		"Zombi",
		"Ghost",
		"Mushroom",
		"Mage",
		"Goblin",
		"Minota",
		"Dragon",
		"PlayerAttackBox",
		"EnemyAttackBox",
		"WarningBox",
		"SpeedItem",
		"HpItem",
		"AttItem",
		"TriCellItem",
		"FourCellItem",
		"RapierItem",
		"ShotGunItem",
		"RifleItem",
		"Cursor",
	};

	for (auto EntityInfoFileName : vecEntityInfoFileName)
	{
		if (!CreateComStruct(EntityInfoFileName))
			return;
	}

	if (!LoadStageEnemyInfo())
		return;
}
//-----------------------------------------------------------------------------------------------------------
void CGameLogicMgr::Update(float fElapsedTime)
{
	if (m_bIsDied)
		return;

	if (m_bIsAllClear)
		return;

	CObjectMgr::getSingleton()->Update(fElapsedTime);

	StageWave(fElapsedTime);
}
//-----------------------------------------------------------------------------------------------------------
void CGameLogicMgr::SetDied(bool bIsDied)
{
	m_bIsDied = bIsDied;

	if (!m_bIsDied)
	{
		m_pPlayer->Do("ResetStat");
		m_pPlayer->Do("ResetSpeed");
		m_pPlayer->SetAttr("IsTempInvincible", true);
		m_pPlayer->Do("FullHp");
		m_pPlayer->SetAttr("Color", m_pPlayer->GetAttr<D2D1::ColorF::Enum>("Color"));
		m_pPlayer->SetAttr("Alpha", 1.f);
	}
}
//-----------------------------------------------------------------------------------------------------------
void CGameLogicMgr::SetStageClear()
{
	if (m_iStageLv == 1)
		return;

	++m_iStageLv;
	m_iWave = 0;
	CreateEnemy();
	++m_iWave;

	m_fWaveTime = 0.f;
	m_bIsAllClear = false;
}
//-----------------------------------------------------------------------------------------------------------
EntityPtr CGameLogicMgr::CreateObject(const std::string& strName, D2D1_POINT_2F Point)
{
	EntityPtr pEntity = EntityFactory::CreateEntity(strName);
	pEntity->SetAttr("Pos", Point);

	CObjectMgr::getSingleton()->AddObject(pEntity);

	return pEntity;
}
//-----------------------------------------------------------------------------------------------------------
void CGameLogicMgr::CreateObjectMsg(const std::string& strName, D2D1_POINT_2F Point)
{
	static std::map<std::string, ENTITY_TYPE> s_mapEntityTypeTable =
	{
		//{ "Player", ENTITY_TYPE::OBJ_PLAYER },
		//{ "OtherPlayer", OBJ_TYPE::OBJ_PLAYER },
		{ "Bat", ENTITY_TYPE::TY_BAT },
		{ "Skeleton", ENTITY_TYPE::TY_SKELETON },
		{ "Zombi", ENTITY_TYPE::TY_ZOMBI },
		{ "Ghost", ENTITY_TYPE::TY_GHOST },
		{ "Mushroom", ENTITY_TYPE::TY_MUSHROOM },
		{ "Mage", ENTITY_TYPE::TY_MAGE },
		{ "Goblin", ENTITY_TYPE::TY_GOBLIN },
		{ "Minota", ENTITY_TYPE::TY_MINOTA },
		{ "Dragon", ENTITY_TYPE::TY_DRAGON },
		{ "PlayerAttackBox", ENTITY_TYPE::TY_ATTACKBOX },
		//{ "PlayerEnemyBox", ENTITY_TYPE::tyen },
		{ "WarningBox", ENTITY_TYPE::TY_WARNINGBOX },
		{ "SpeedItem", ENTITY_TYPE::TY_SPEEDUP },
		{ "HpItem", ENTITY_TYPE::TY_HP },
		{ "AttItem", ENTITY_TYPE::TY_ATT },
		{ "TriCellItem", ENTITY_TYPE::TY_TRICELL },
		{ "FourCellItem", ENTITY_TYPE::TY_FOURCELL },
		{ "RapierItem", ENTITY_TYPE::TY_RAPIER },
		{ "ShotGunItem", ENTITY_TYPE::TY_SHOTGUN },
		{ "RifleItem", ENTITY_TYPE::TY_RIFLE },
		//{ "Cursor", ENTITY_TYPE::ty },
	};

	auto entityTypeIter = s_mapEntityTypeTable.find(strName);

	// todo : SMsgSend msgSend(PACKET_ID::PACKET_NEW_OBJ);
	/*
	if (entityTypeIter != s_mapEntityTypeTable.end())
	{
		ENTITY_TYPE eType = entityTypeIter->second;
		msgSend << entityTypeIter->second << Point.x << Point.y;
	}

	CMyMain::getSingleton()->GetSessionPtr()->Send(msgSend);
	*/
}
//-----------------------------------------------------------------------------------------------------------
bool CGameLogicMgr::EnemyEmptyCheck()
{
	auto isEnemy = [](EntityPtr pEntity) -> bool
	{
		return (pEntity->GetObjType() == OBJ_TYPE::OBJ_ENEMY);
	};

	if (CObjectMgr::getSingleton()->find_for(isEnemy) != nullptr)
		return false;
	else
		return true;
}
//-----------------------------------------------------------------------------------------------------------
void CGameLogicMgr::CreateEnemy()
{
	for (auto enemy : m_vecEnemyWaveInfo[m_iStageLv][m_iWave])
	{
		CreateObjectMsg(enemy.strEnemyType, enemy.tPos);
	}
}
//-----------------------------------------------------------------------------------------------------------
void CGameLogicMgr::StageWave(float fElapsedTime)
{
	if (!m_bIsHost)
		return;

	m_fWaveTime += fElapsedTime;

	switch (m_iWave)
	{
	case 1:
		if (m_fWaveTime > 4.f)
		{
			CreateEnemy();
			m_fWaveTime = 0.f;
			++m_iWave;
		}
		break;
	case 2:
		if (m_fWaveTime > 5.f)
		{
			CreateEnemy();
			m_fWaveTime = 0.f;
			++m_iWave;
		}
		break;
	case 3:
		if (m_fWaveTime > 10.f)
		{
			CreateEnemy();
			m_fWaveTime = 0.f;
			++m_iWave;
		}
		break;
	case 4:
		if (EnemyEmptyCheck())
			m_bIsAllClear = true;
		break;
	default:
		break;
	}
}
//-----------------------------------------------------------------------------------------------------------
bool CGameLogicMgr::CreateComStruct(const char* szName)
{
	std::string strFileName = "Data/";
	strFileName += szName;
	strFileName += ".json";
	rapidjson::Document document = json_loader::file_load(strFileName.c_str());

	if (document.HasParseError())
		return false;

	std::vector<EntityFactory::ComsIDAndInitVal> vecComsStruct;

	rapidjson::Value::ConstMemberIterator iter = document.MemberBegin();
	rapidjson::Value::ConstMemberIterator enditer = document.MemberEnd();

	std::string strEntityName = iter->value.GetString();
	++iter;

	for (; iter != enditer; ++iter)
	{
		EntityFactory::ComsIDAndInitVal comStruct;
		comStruct.strComID = iter->name.GetString();

		const rapidjson::Value& memberValue = iter->value;
		rapidjson::Value::ConstMemberIterator memberEndIter = memberValue.MemberEnd();
		for (rapidjson::Value::ConstMemberIterator memberIter = memberValue.MemberBegin(); memberIter != memberEndIter; ++memberIter)
		{
			boost::any anyValue;
			const rapidjson::Value& initValue = memberIter->value;

			std::string strMemberValueName = memberIter->name.GetString();

			if (strMemberValueName == "Shape")
			{
				static std::map<std::string, RENDER_VALUE> s_mapRenderValueTable =
				{
					{ "E_CIRCLE", RENDER_VALUE::E_CIRCLE },
					{ "E_CIRCLE_N", RENDER_VALUE::E_CIRCLE_N },
					{ "E_RECT", RENDER_VALUE::E_RECT },
					{ "E_RECT_N", RENDER_VALUE::E_RECT_N },
				};

				auto renderValueIter = s_mapRenderValueTable.find(initValue.GetString());

				if (renderValueIter != s_mapRenderValueTable.end())
					anyValue =renderValueIter->second;
			}
			else if (strMemberValueName == "ColObjType")
			{
				static std::map<std::string, OBJ_TYPE> s_mapEntityColObjTypeTable =
				{
					{ "ENTITY_PLAYER", OBJ_TYPE::OBJ_PLAYER },
					{ "ENTITY_ENEMY", OBJ_TYPE::OBJ_ENEMY },
					{ "ENTITY_PLAYERATTACKBOX", OBJ_TYPE::OBJ_PLAYERATTACKBOX },
					{ "ENTITY_ENEMYATTACKBOX", OBJ_TYPE::OBJ_ENEMYATTACKBOX },
					{ "ENTITY_WARNINGBOX", OBJ_TYPE::OBJ_WARNINGBOX },
					{ "ENTITY_ITEM", OBJ_TYPE::OBJ_ITEM },
				};

				auto entityColObjTypeIter = s_mapEntityColObjTypeTable.find(initValue.GetString());

				if (entityColObjTypeIter != s_mapEntityColObjTypeTable.end())
					anyValue = entityColObjTypeIter->second;
			}
			else if (initValue.IsInt())
				anyValue = initValue.GetInt();
			else if (initValue.IsDouble())
				anyValue = static_cast<float>(initValue.GetDouble());
			else if (initValue.IsString())
				anyValue = static_cast<std::string>(initValue.GetString());
			else if (initValue.IsBool())
				anyValue = initValue.GetBool();

			comStruct.InitVal.emplace(dynamic::value_type(memberIter->name.GetString(), anyValue));
		}
		vecComsStruct.push_back(comStruct);
	}
	EntityFactory::m_mapComponentStruct.emplace(EntityFactory::ENTITY_MAP::value_type(strEntityName, vecComsStruct));

	return true;
}
//-----------------------------------------------------------------------------------------------------------
bool CGameLogicMgr::LoadStageEnemyInfo()
{
	rapidjson::Document document = json_loader::file_load("Data/StageEnemyInfo.json");

	if (document.HasParseError())
		return false;

	auto iter = document.FindMember("Enemies");

	int nCount = iter->value.Size();

	for (int i = 0; i < nCount; ++i)
	{
		if (iter->value[i].IsObject())
			CreateStageEnemyInfo(iter->value[i]);
	}
	return true;
}
//-----------------------------------------------------------------------------------------------------------
void CGameLogicMgr::CreateStageEnemyInfo(const rapidjson::Value& value)
{
	rapidjson::Value::ConstMemberIterator iter = value.MemberBegin();
	rapidjson::Value::ConstMemberIterator enditer = value.MemberEnd();
	++iter;

	std::vector<std::vector<EnemyCreateInfo>> vecEnemyStageInfo;

	for (; iter != enditer; ++iter)
	{
		rapidjson::Value::ConstValueIterator waveEndIter = iter->value.End();

		std::vector<EnemyCreateInfo> vecEnemyInfo;

		for (rapidjson::Value::ConstValueIterator waveIter = iter->value.Begin(); waveIter != waveEndIter; ++waveIter)
		{
			EnemyCreateInfo stEnemyInfo;

			rapidjson::Value::ConstMemberIterator infoEndIter = waveIter->MemberEnd();
			for (rapidjson::Value::ConstMemberIterator infoIter = waveIter->MemberBegin(); infoIter != infoEndIter; ++infoIter)
			{
				std::string strName = infoIter->name.GetString();
				const rapidjson::Value& infoValue = infoIter->value;

				if (strName == "Type")
					stEnemyInfo.strEnemyType = infoValue.GetString();
				else if (strName == "PosRandom")
					stEnemyInfo.tPos = RandomPoint();
				else if (strName == "PosX")
					stEnemyInfo.tPos.x = infoValue.GetDouble();
				else if (strName == "PosY")
					stEnemyInfo.tPos.y = infoValue.GetDouble();
			}
			vecEnemyInfo.push_back(stEnemyInfo);
		}
		vecEnemyStageInfo.push_back(vecEnemyInfo);
	}
	m_vecEnemyWaveInfo.push_back(vecEnemyStageInfo);
}