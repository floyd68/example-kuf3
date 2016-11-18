local attackTime = 0.0

function Update(time_value)
	if(AIClass:Ready(time_value) == false) then
		return
	end
	
	attackTime = attackTime + time_value
	
	if(attackTime > 2.0) then
		AIClass:DoAttack2(1)
		attackTime = 0.0
	end
end