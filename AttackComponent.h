#pragma once

#include "Entity.h"

struct AttackOption;

class CAttackComponent : public CComponentBase
{
public:
	explicit CAttackComponent(EntityPtr pEntity, const dynamic& values)
		: CComponentBase(pEntity) {}
	virtual ~CAttackComponent() {}

	DEFINE_DO_ATTR(CAttackComponent)

private:
	DO_ACTION

	virtual COMPONENT_ID GetComponentID() const { return COMPONENT_ID::COM_ATTACK; }

private:
	bool DoAtt(const boost::any& param);
	bool DoAttWarning(const boost::any& param);
	bool DoAttAllWay(const boost::any& param);

private:
	AttackOption GetAttackOption() const;
};