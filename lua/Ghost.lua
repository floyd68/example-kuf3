local GHOST_CHASE = 0
local GHOST_INVISIBLE = 1

local Color_AntiqueWhite = 16444375
local Color_WhiteSmoke = 16119285

local ghostState = GHOST_CHASE
local ghostTime = 0.0

function Update(time_value)
	if(AIClass:Ready(time_value) == false) then
		return
	end
	
	ghostTime = ghostTime + time_value
	
	if(ghostState == GHOST_CHASE) then
		AIClass:Chase()
		
		if(ghostTime > 2.0) then
			ghostTime = 0.0
			if(math.random(0, 1) == 1) then
				AIClass:SetSpeed(0.0)
				AIClass:SetColor(Color_AntiqueWhite)
				AIClass:SetInvincible(true)
				ghostState = GHOST_INVISIBLE
			end
		end
	elseif(ghostState == GHOST_INVISIBLE) then
		if(ghostTime > 2.0) then
			AIClass:SetSpeed(100.0)
			AIClass:SetColor(Color_WhiteSmoke)
			AIClass:SetInvincible(false)
			ghostTime = 0.0
			ghostState = GHOST_CHASE
		end
	end
end