--init here
-- thiz: current entity id
print "collision test"
function onInit(thiz)

	initHealth = 100
	setMaxHealth(thiz, initHealth)
	setCurrentHealth(thiz, initHealth)
	print ("Current HP: " .. getCurrentHealth(thiz))
	is_collided = true
end


--Update the script here
-- thiz: current entity id
-- dt: dealt time
function onUpdate(thiz, dt)
	
	current_health = getCurrentHealth(thiz)  --tracking current health
	
	--tracking collide state
	num_collided_ent =  getNumCollidedEntities(thiz)
	--print(num_collided_ent)
	for i = 0, 	num_collided_ent-1 do 
		collided_entity = getCollidedEntity(thiz, i)
		--print("ent id : " .. collided_entity)
	end
		
	if num_collided_ent ~= 0 and is_collided == true then 
		current_health = current_health - 10
		setCurrentHealth(thiz, current_health)
		Qwarn("attack! HP loss: 10")
		Qwarn("current_HP: " .. current_health)
		Audio.playClip("laser1")
		is_collided = false
	end
	
	if num_collided_ent == 0 then
		is_collided = true
	end
		
end

