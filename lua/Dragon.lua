local DRAGON_CHASE = 0
local DRAGON_BREATH = 1

local dragonState = DRAGON_CHASE
local breathDelay = 0.0

function Update(time_value)
	if(AIClass:Ready(time_value) == false) then
		return
	end
	
	breathDelay = breathDelay + time_value
	
	if(dragonState == DRAGON_CHASE) then
		AIClass:Chase()
		if(breathDelay > 1.5) then
			AIClass:SetSpeed(0.0)
			breathDelay = 0.0
			dragonState = DRAGON_BREATH
			if(AIClass:RangeCheck() < 300.0) then --브레스는 다각형되면 만들기
				AIClass:DelayAttack("ATT_FIRE")
				AIClass:AttackToTarget()
			else
				AIClass:DelayAttack("ATT_ONEWAY")
				AIClass:AttackToTarget()
			end
		end
	elseif(dragonState == DRAGON_BREATH) then
		if(breathDelay > 1.0) then
			breathDelay = 0.0
			AIClass:SetSpeed(40.0)
			dragonState = DRAGON_CHASE
		end
	end
end