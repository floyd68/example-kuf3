#include "stdafx.h"
#include "EntityFactory.h"

#include "PositionComponent.h"
#include "RenderComponent.h"
#include "ColliderComponent.h"
#include "AttackComponent.h"
#include "InputComponent.h"
#include "CreateComponent.h"
#include "WarningBoxComponent.h"
#include "GuageComponent.h"
#include "ItemComponent.h"
#include "StatPlayerComponent.h"
#include "StatEnemyComponent.h"
#include "AttackBoxComponent.h"
#include "AIComponent.h"
#include "CursorComponent.h"
#include "ObjectMgr.h"

EntityFactory::ENTITY_MAP EntityFactory::m_mapComponentStruct = {};
//-----------------------------------------------------------------------------------------------------------
typedef CompPtr(*CompCreatorfunc)(EntityPtr, const dynamic&);

#define COMP( _name_ ) [](EntityPtr pEntity, const dynamic& params) -> CompPtr { return CompPtr(new _name_(pEntity, params)); }

static std::map <std::string, CompCreatorfunc> s_mapCompCreator =
{
	{ "COM_POS", COMP(CPositionComponent) }, // svr
	{ "COM_STATPLAYER", COMP(CStatPlayerComponent) }, // svr
	{ "COM_STATENEMY", COMP(CStatEnemyComponent) }, // svr
	{ "COM_RENDER", COMP(CRenderComponent) },
	{ "COM_COLLIDER", COMP(CColliderComponent) }, //  충돌 // 클라가 하자
	{ "COM_PLAYERATTACKBOX", COMP(CAttackBoxComponent) },
	{ "COM_ENEMYATTACKBOX", COMP(CAttackBoxComponent) },
	{ "COM_ATTACK", COMP(CAttackComponent) },
	{ "COM_INPUT", COMP(CInputComponent) },
	{ "COM_CREATE", COMP(CCreateComponent) },
	{ "COM_AI", COMP(CAIComponent) },
	{ "COM_WARNINGBOX", COMP(CWarningBoxComponent) },
	{ "COM_GUAGE", COMP(CGuageComponent) },
	{ "COM_ITEM", COMP(CItemComponent) },
	{ "COM_CURSOR", COMP(CCursorComponent) },
};
//-----------------------------------------------------------------------------------------------------------
static std::map<std::string, OBJ_TYPE> s_mapEntityObjTypeTable =
{
	{ "Player", OBJ_TYPE::OBJ_PLAYER },
	{ "OtherPlayer", OBJ_TYPE::OBJ_PLAYER },
	{ "Bat", OBJ_TYPE::OBJ_ENEMY },
	{ "Skeleton", OBJ_TYPE::OBJ_ENEMY },
	{ "Zombi", OBJ_TYPE::OBJ_ENEMY },
	{ "Ghost", OBJ_TYPE::OBJ_ENEMY },
	{ "Mushroom", OBJ_TYPE::OBJ_ENEMY },
	{ "Mage", OBJ_TYPE::OBJ_ENEMY },
	{ "Goblin", OBJ_TYPE::OBJ_ENEMY },
	{ "Minota", OBJ_TYPE::OBJ_ENEMY },
	{ "Dragon", OBJ_TYPE::OBJ_ENEMY },
	{ "PlayerAttackBox", OBJ_TYPE::OBJ_PLAYERATTACKBOX },
	{ "PlayerEnemyBox", OBJ_TYPE::OBJ_ENEMYATTACKBOX },
	{ "WarningBox", OBJ_TYPE::OBJ_WARNINGBOX },
	{ "SpeedItem", OBJ_TYPE::OBJ_ITEM },
	{ "HpItem", OBJ_TYPE::OBJ_ITEM },
	{ "AttItem", OBJ_TYPE::OBJ_ITEM },
	{ "TriCellItem", OBJ_TYPE::OBJ_ITEM },
	{ "FourCellItem", OBJ_TYPE::OBJ_ITEM },
	{ "RapierItem", OBJ_TYPE::OBJ_ITEM },
	{ "ShotGunItem", OBJ_TYPE::OBJ_ITEM },
	{ "RifleItem", OBJ_TYPE::OBJ_ITEM },
	{ "Cursor", OBJ_TYPE::OBJ_CURSOR },
};
//-----------------------------------------------------------------------------------------------------------
static std::map<std::string, dynamic> s_EntityStrAttrTable =
{
	{ "Player", { { "Speed", 200.f }, { "Hp", 100.f }, { "CurHp", 100.f }, { "Att", 10.f } } },
	{ "OtherPlayer", { { "Speed", 200.f }, { "Hp", 100.f }, { "CurHp", 100.f }, { "Att", 10.f } } },
	{ "Bat", { { "Speed", 60.f }, { "Hp", 20.f }, { "CurHp", 20.f }, { "Att", 10.f } } },
	{ "Skeleton", { { "Speed", 60.f }, { "Hp", 30.f }, { "CurHp", 30.f }, { "Att", 10.f } } },
	{ "Zombi", { { "Speed", 60.f }, { "Hp", 30.f }, { "CurHp", 30.f }, { "Att", 10.f } } },
	{ "Ghost", { { "Speed", 100.f }, { "Hp", 20.f }, { "CurHp", 20.f }, { "Att", 20.f } } },
	{ "Mushroom", { { "Speed", 0.f }, { "Hp", 40.f }, { "CurHp", 40.f }, { "Att", 20.f } } },
	{ "Mage", { { "Speed", 20.f }, { "Hp", 50.f }, { "CurHp", 50.f }, { "Att", 10.f } } },
	{ "Goblin", { { "Speed", 80.f }, { "Hp", 30.f }, { "CurHp", 30.f }, { "Att", 10.f } } },
	{ "Minota", { { "Speed", 50.f }, { "Hp", 100.f }, { "CurHp", 100.f }, { "Att", 30.f } } },
	{ "Dragon", { { "Speed", 50.f }, { "Hp", 100.f }, { "CurHp", 100.f }, { "Att", 40.f } } },
	{ "PlayerAttackBox", { { "Alpha", 0.5f }, { "Color", D2D1::ColorF::Blue } } },
	{ "EnemyAttackBox", { { "Alpha", 0.5f } } },
	{ "WarningBox", {} },
	{ "SpeedItem", { { "Item", ITEM_ID::ITEM_SPEEDUP } } },
	{ "HpItem", { { "Item", ITEM_ID::ITEM_HP } } },
	{ "AttItem", { { "Item", ITEM_ID::ITEM_ATT } } },
	{ "TriCellItem", { { "Item", ITEM_ID::ITEM_TRICELL } } },
	{ "FourCellItem", { { "Item", ITEM_ID::ITEM_FOURCELL } } },
	{ "RapierItem", { { "Item", ITEM_ID::ITEM_RAPIER } } },
	{ "ShotGunItem", { { "Item", ITEM_ID::ITEM_SHOTGUN } } },
	{ "RifleItem", { { "Item", ITEM_ID::ITEM_RIFLE } } },
	{ "Cursor", {} },
};
//-----------------------------------------------------------------------------------------------------------
EntityPtr EntityFactory::CreateEntity(const std::string& strName)
{
	EntityPtr pEntity = std::make_shared<CEntity>();

	size_t uObjSize = CObjectMgr::getSingleton()->GetvecObjSize() + CObjectMgr::getSingleton()->GetvecAddObjSize();

	pEntity->SetEntityID(uObjSize);

	auto iter = m_mapComponentStruct.find(strName);
	if (iter == m_mapComponentStruct.end())
		return nullptr;

	auto entityObjTypeIter = s_mapEntityObjTypeTable.find(strName);

	if (entityObjTypeIter != s_mapEntityObjTypeTable.end())
		pEntity->SetObjType(entityObjTypeIter->second);

	const std::vector<ComsIDAndInitVal>& EntityDef = iter->second;

	for (auto ComsIDAndValue : EntityDef)
	{
		auto it = s_mapCompCreator.find(ComsIDAndValue.strComID);
		if (it != s_mapCompCreator.end())
			pEntity->InsertComponent(ComsIDAndValue.strComID, it->second(pEntity, ComsIDAndValue.InitVal));
	}

	auto strAttrIter = s_EntityStrAttrTable.find(strName);
	if (strAttrIter == s_EntityStrAttrTable.end())
		return nullptr;

	for (auto strAttr : strAttrIter->second)
	{
		pEntity->SetAttr(strAttr.first, strAttr.second);
	}

	return pEntity;
}