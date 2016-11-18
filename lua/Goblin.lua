local GOBLIN_CHASE = 0
local GOBLIN_RUNAWAY = 1

local Color_LightBlue = 11393254
local Color_DarkBlue = 139

local goblinState = GOBLIN_CHASE

function Update(time_value)
	if(AIClass:Ready(time_value) == false) then
		return
	end
	
	if(goblinState == GOBLIN_CHASE) then
		AIClass:Chase()
		if(AIClass:IsUnderAttack() == true) then
			AIClass:SetSpeed(-150.0)
			AIClass:SetColor(Color_LightBlue)
			goblinState = GOBLIN_RUNAWAY
		end
	end
end

function OutScreen()
	if(goblinState == GOBLIN_RUNAWAY) then
		AIClass:SetSpeed(80.0)
		AIClass:SetColor(Color_DarkBlue)
		AIClass:SetInvincible(false)
		goblinState = GOBLIN_CHASE
	end
end