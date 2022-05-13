--init here
-- thiz: current entity id
print "3d test"
function onInit(thiz)
	
	camera = getCamera()
	Audio.setLooping(true)
	Audio.playClip("3d_test")
	
	-- tricky here, bind 
	pos = getPosition(thiz)
	Audio.active3DEffect(pos)
	
end


--Update the script here
-- thiz: current entity id
-- dt: dealt time
v = vec3(0,0,0)
function onUpdate(thiz, dt)
		
	if Input.get_key_pressed(Key.Q_KEY_D) then 
		Listener.setOritentation(1,0,0,0,0,1)
		v.x = v.x + 1
	end	
	
	if Input.get_key_pressed(Key.Q_KEY_A) then 
		Listener.setOritentation(-1,0,0,0,0,1)
		v.x = v.x - 1
	end	
	
	if Input.get_key_pressed(Key.Q_KEY_W) then 
		Listener.setOritentation(0,1,0,0,0,1)
		v.y = v.y + 1
	end	
	
	if Input.get_key_pressed(Key.Q_KEY_S) then 
		Listener.setOritentation(0,-1,0,0,0,1)
		v.y = v.y -1
	end	
	
	if Input.get_key_pressed(Key.Q_KEY_Q) then 
		Audio.setLooping(false)
		Audio.stopClip()
	end	
				
	updatePosition(camera, v)
	Listener.setPositionv(v)
		
end
