--Update the script here
--thiz: current entity id

print "mouse check"
function onUpdate(thiz)

	if Input.get_mouse_clicked(Mouse.Q_MOUSE_LEFT) then 
		Audio.playClip("laser1")
	end	
	
	if Input.get_mouse_clicked(Mouse.Q_MOUSE_RIGHT) then 
		Audio.playClip("message1")
	end	
	
end





