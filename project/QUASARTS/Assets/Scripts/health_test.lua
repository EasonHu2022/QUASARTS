--Update the script here
-- thiz: current entity id
-- dt: dealt time
print "health test"


function onInit(thiz)
	initHealth = 100
	setMaxHealth(thiz, initHealth)
	setCurrentHealth(thiz, initHealth)
	print ("init: " .. getCurrentHealth(thiz))
	
end	

function onUpdate(thiz, dt)
	
	current_health = getCurrentHealth(thiz)
	
	if Input.get_key_pressed(Key.Q_KEY_D) then 
		current_health = current_health + 10
		setCurrentHealth(thiz, current_health)
		print ("update: " .. getCurrentHealth(thiz))
	end	
	
	if Input.get_key_pressed(Key.Q_KEY_A) then 
		current_health = current_health - 10
		setCurrentHealth(thiz, current_health)
		print ("update: " .. getCurrentHealth(thiz))
	end	
	
end
