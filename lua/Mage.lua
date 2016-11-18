local MAGE_CHASE = 0
local MAGE_ATTACK = 1

local Color_Purple = 8388736
local Color_MediumPurple = 9662683

local mageState = MAGE_CHASE
local attackTime = 0.0

function Update(time_value)
	if(AIClass:Ready(time_value) == false) then
		return
	end
	
	if(mageState == MAGE_CHASE) then
		AIClass:Chase()
		if(AIClass:RangeCheck() < 300.0) then
			AIClass:SetSpeed(0.0)
			AIClass:SetColor(Color_Purple)
			mageState = MAGE_ATTACK
		end
	elseif(mageState == MAGE_ATTACK) then
		attackTime = attackTime + time_value
		if(attackTime > 1.0) then
			AIClass:AttackToTarget()
			attackTime = 0.0
		end
		
		if(AIClass:RangeCheck() > 300.0) then
			AIClass:SetSpeed(20.0)
			AIClass:SetColor(Color_MediumPurple)
			attackTime = 0.0
			mageState = MAGE_CHASE
		end
	end
	
end