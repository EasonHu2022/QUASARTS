-- Init here
-- thiz: current entity ID
function onInit(thiz)

	is_collided = true
	velocity = 0
	acceleration = 10

end

-- Update the script here
-- thiz: current entity ID
-- dt: dealt time
function onUpdate(thiz, dt)

	-- Update acceleration and velocity:
	acceleration = acceleration + (75 * dt)
	velocity = velocity - (acceleration * dt)

	-- If the entity has not collided, move it:
	if is_collided == true then
		v = vec3(0,0,velocity * dt)
		updatePosition(thiz, v)
	end

	-- Tracking collide state:
	num_collided_ent =  getNumCollidedEntities(thiz)

	-- Reduce health of collided entities:
	for i = 0, num_collided_ent-1 do 
		collided_entity = getCollidedEntity(thiz, i)
		setCurrentHealth(collided_entity, 0);
	end

	-- Reduce health of current entity:
	if num_collided_ent ~= 0 and is_collided == true then 
		setCurrentHealth(thiz, 0)
		Qwarn("BOOM!")
		Audio.playClip("explosion5")
		is_collided = false
	end

	if num_collided_ent == 0 then
		is_collided = true
	end

end
