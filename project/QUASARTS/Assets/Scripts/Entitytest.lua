--init here
-- thiz: current entity id
function onInit(thiz)
	id = createEntity("sphere")
	Qwarn("id: ".. id)
	addComponent(id, "Health")
	initHealth = 100
	setMaxHealth(id, initHealth)
	setCurrentHealth(id, initHealth)
	print ("init: " .. getCurrentHealth(id))
end

--Update the script here
-- thiz: current entity id
-- dt: dealt time
function onUpdate(thiz, dt)
	
	current_health = getCurrentHealth(id)
	local v = vec3(0,0,0)
	
	if Input.get_key_pressed(Key.Q_KEY_D) then 
		current_health = current_health + 10
		setCurrentHealth(id, current_health)
		print ("update: " .. getCurrentHealth(id))
		v.x = v.x + 1		
	end	
	
	if Input.get_key_pressed(Key.Q_KEY_A) then 
		current_health = current_health - 10
		setCurrentHealth(id, current_health)
		print ("update: " .. getCurrentHealth(id))
		v.x = v.x - 1
	end	
		
	updatePosition(thiz, v)
end




