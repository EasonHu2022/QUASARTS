--init here
-- thiz: current entity id
print "camera test"
function onInit(thiz)
	
end


camera = getCamera()

--Update the script here
-- thiz: current entity id
-- dt: dealt time
function onUpdate(thiz, dt)
	
	if Input.get_key_held(Key.Q_KEY_Q) then
		cameraZoomIn(camera)
	end
	
	if Input.get_key_held(Key.Q_KEY_E) then
		cameraZoomOut(camera)
	end
	
	local v = vec3(0,0,0)
	if Input.get_key_held(Key.Q_KEY_D) then 
		v.x = v.x + 1
		
	end	
	
	if Input.get_key_held(Key.Q_KEY_A) then 
		v.x = v.x - 1
	end	
	
	if Input.get_key_held(Key.Q_KEY_W) then 
		v.z = v.z - 1
	end	
	
	if Input.get_key_held(Key.Q_KEY_S) then 
		v.z = v.z + 1
	end	
	
	updatePosition(camera, v)
	
	
	
end
