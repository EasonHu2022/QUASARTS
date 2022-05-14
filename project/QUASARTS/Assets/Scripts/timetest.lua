--Update the script here
--thiz: current entity id
print("time test")
timer = 9
function onUpdate(thiz, dt)


	timer = timer + dt
	v = vec3(0,0,0)
	if timer > 10 then 	
		v.x = v.x + dt
		
	end 
	updatePosition(thiz, v)
end





