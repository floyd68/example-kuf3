function Update(time_value)
	if(AIClass:Ready(time_value) == false) then
		return
	end
end

function OutScreen()
	AIClass:ReturnBack()
end