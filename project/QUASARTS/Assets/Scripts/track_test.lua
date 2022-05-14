--Update the script here
--thiz: current entity id

print "sound check"
function onUpdate(thiz)

	if Input.get_mouse_clicked(Mouse.Q_MOUSE_LEFT) then 
		Audio.playTrack("b423b42")
	end	
	
	if Input.get_mouse_clicked(Mouse.Q_MOUSE_RIGHT) then 
		Audio.pauseTrack()
	end	
	
	if Input.get_key_pressed(Key.Q_KEY_A) then
		Audio.resumeTrack()
	end
end





