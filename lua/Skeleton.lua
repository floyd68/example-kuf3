function Update(time_value)
	if(AIClass:Ready(time_value) == false) then
		return
	end
	
	AIClass:Chase()
end