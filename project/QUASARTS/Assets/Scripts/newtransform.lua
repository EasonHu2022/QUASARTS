--Update the script here
--thiz: current entity id

print("new transform")
function onUpdate(thiz)
	
	local v = vec3(0,0,0)
	if Input.get_key_pressed(Key.Q_KEY_D) then 
		v.x = v.x + 1
	end	
	
	if Input.get_key_pressed(Key.Q_KEY_A) then 
		v.x = v.x - 1
	end	
	
	if Input.get_key_pressed(Key.Q_KEY_W) then 
		v.y = v.y + 1
	end	
	
	if Input.get_key_pressed(Key.Q_KEY_S) then 
		v.y = v.y -1
	end	
	
	updatePosition(thiz, v)
end





