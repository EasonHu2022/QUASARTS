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
	
	if Input.get_key_held(Key.Q_KEY_W) then
		cameraZoomIn(camera)
	end
	
	if Input.get_key_held(Key.Q_KEY_S) then
		cameraZoomOut(camera)
	end
	
	updateRotation(camera, vec3(0,0.4,0))
	
end
