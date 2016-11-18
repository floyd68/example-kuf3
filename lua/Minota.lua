local MINOTA_CHASE = 0
local MINOTA_CHARGE = 1
local MINOTA_STUN = 2

local Color_SandyBrown = 16032864
local Color_SaddleBrown = 9127187
local Color_Brown = 10824234

local minotaState = MINOTA_CHASE
local minotaTime = 0.0

function Update(time_value)
	if(AIClass:Ready(time_value) == false) then
		return
	end
	
	minotaTime = minotaTime + time_value
	
	if(minotaState == MINOTA_CHASE) then
		AIClass:Chase()
		if(minotaTime > 3.0) then
			AIClass:SetSpeed(400.0)
			minotaState = MINOTA_CHARGE
		end
	elseif(minotaState == MINOTA_CHARGE) then
		
	elseif(minotaState == MINOTA_STUN) then
		if(minotaTime > 1.0) and (minotaTime < 2.0) then
			AIClass:SetColor(Color_SaddleBrown)
		elseif(minotaTime > 2.0) then
			AIClass:SetSpeed(50.0)
			AIClass:SetColor(Color_Brown)
			minotaTime = 0.0
			minotaState = MINOTA_CHASE
		end
	end
end

function OutScreen()
	if(minotaState == MINOTA_CHARGE) then
		AIClass:SetSpeed(0.0)
		AIClass:SetColor(Color_SandyBrown)
		minotaTime = 0.0
		minotaState = MINOTA_STUN
	end
end