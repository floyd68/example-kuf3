local randomMoveTime = 0.0

function Update(time_value)
	if(AIClass:Ready(time_value) == false) then
		return
	end
	
	randomMoveTime = randomMoveTime + time_value
	
	if(randomMoveTime > 1.0) then
		AIClass:ChangeRandomDir()
		randomMoveTime = 0.0
	end
end

function OutScreen()
	if(randomMoveTime > 1.0) then
		AIClass:ChangeRandomDir()
		randomMoveTime = 0.0
	end
end