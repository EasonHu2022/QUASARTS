# Script System
For Quasarts Engine, we select lua as our scripting language

## How to add and load a script
1. Create an entity first
2. Click the entity which you want to bind a script component in Asset Hierarchy window
3. Add: select the menu bar: **Assets -> Add Script Component** and choose the lua file you want to use
4. Load and Stop: select the menu bar: **Edit -> Play/Stop**

## Update tracking

To init attributes, please put everything you want to initialize in `onInit` function

To keep tracking the content in script, please put everything you want to update in `onUpdate` function

``` lua
--init here
-- thiz: current entity id
function onInit(thiz)
end

--Update the script here
-- thiz: current entity id
-- dt: dealt time
function onUpdate(thiz, dt)
end
```


## Current Script API
- Input
- Math
- Audio
- Log
- New Entity
- Components
  - Transform
  - Health 
  - Weapon
  - Camera
  - Collision
- Time

## Input 

Function | Argument 
---------| ---------
Input.get_key_held       |  keycodes(int)
Input.get_key_pressed    |  keycodes(int)
Input.get_key_released   |  keycodes(int)
Input.get_mouse_held     |  mousecodes(int)
Input.get_mouse_clicked  |  mousecodes(int)
Input.get_mouse_released |  mousecodes(int)


1.For key codes, we use the format like `Q_KEY_X`

2.For mouse codes, we use the format like `Q_MOUSE_X`

### [keycodes and mousecodes list](./InputExporter.cpp)

### simple example

``` lua
if Input.get_key_held(Key.Q_KEY_A) then
    --do what you want to do here
end

if Input.get_mouse_held(Mouse.Q_MOUSE_LEFT) then
    --do what you want to do here
end
```

## Math(from glm library)

Quasarts Engine now supports:
- vec2
- vec3
- vec4
- mat3
- mat4


### simple example

``` lua
-- create a variable
v2 = vec2(2,3)

-- get a diagonal matrix
m3 = mat3(3) 
-- or you can set every value of the matrix
m3 = mat3(1,2,3,4,5,6,7,8,9)
```

## Audio

Quasarts Engine supports playing sound clip(***.ogg format only***) and long track (***.wav format only***)

Function | Argument
---------| ---------
Audio.playClip       | source name(string)
Audio.pauseClip      | /
Audio.stopClip       | /
Audio.resumeClip     | /
Audio.setLooping     | looping_state(bool)
Audio.playTrack      | source name(string)
Audio.pauseTrack     | /
Audio.stopTrack      | /
Audio.resumeTrack    | /
Audio.active3DEffect | audio_source_position(glm::vec3)


Function | Arguments | Description
---------| --------- | ---------
Listener.setAttunation       |  /                            | Set distance attenuation model
Listener.setPosition3f       |  x, y, z (float)              | Set listener position with separate x, y, z
Listener.setPositionv        |  vec3                         | Set listener position with vec3
Listener.setOritentation3f   |  x, y, z 		     | Set listener oritentation with separate x, y, z
Listener.setOritentationv    |  vec3                         | Set listener oritentation with vec3

## Log

Quasarts Engine supports log functions

Function  |
--------- |
Qlog      |
Qtrace    |
Qerror    |
Qwarn     |
Glog      |
Gtrace    |
Gerror    |
Gwarn     |


### simple example

``` lua
Qlog("log")
v = 2
Qwarn("v = " .. v)
```

## New Entity

Quasarts Engine supports creating new entitiy via script, also bind components 

Function | Argument1 | Argument2
---------| --------- | ---------
createEntity   | entity_name(string)     | \
addComponent   | entity_id(unsigned int) | component_type(string)

### simple example

``` lua
id = createEntity("new_entity")
addComponent(id, "Health")
```



## Components

Quasarts Engine can manipulate other components which belong to current entity

### Transform

Function | Argument1 | Argument2
---------| --------- | ---------
updatePosition    | current_entity_id(unsigned int) | step(vec3)
getPosition       | current_entity_id(unsigned int) | /
updateRotation    | current_entity_id(unsigned int) | step(vec3)
updateScale       | current_entity_id(unsigned int) | step(vec3)


**Note** : current_entity_id is passed from C++ side, so just keep it as `thiz`. If you create a new entity in script, then just use the new entity id


#### simple example

``` lua
local dP = vec3(0,0,0)
local dR = vec3(0,0,0)
local dS = vec3(0,0,0)
updatePosition(thiz, dP)
updateRotation(thiz, dR)
updateScale(thiz, dS)
```


### Health

Function | Argument1 | Argument2
---------| --------- | ---------
setMaxHealth       | current_entity_id(unsigned int) | max_health(float)
setCurrentHealth   | current_entity_id(unsigned int) | current_health(float)
getCurrentHealth   | current_entity_id(unsigned int) | /

**Note** : current_entity_id is passed from C++ side, so just keep it as `thiz`. If you create a new entity in script, then just use the new entity id


#### simple example

``` lua
init_health = 100
setMaxHealth(thiz, init_health)
setsetCurrentHealth(thiz, init_health)
current_health = entity.getCurrentHealth(thiz)
current_health = current_health - 20 -- change the health
setsetCurrentHealth(thiz, current_health)
```


### Weapon

Function | Argument1 | Argument2
---------| --------- | ---------
setDamage  | current_entity_id(unsigned int) | damage(float)
setRange   | current_entity_id(unsigned int) | range(float)
setSpeed   | current_entity_id(unsigned int) | speed(int)

**Note** : current_entity_id is passed from C++ side, so just keep it as `thiz`. If you create a new entity in script, then just use the new entity id


#### simple example

``` lua
setDamage(thiz, 20)
setRange(thiz, 5)
setSpeed(thiz, 2)
```

### Camera

Function | Argument1 
---------| --------- 
getCamera      | /
cameraZoomIn   | camera id(unsigned int)
cameraZoomOut  | camera id(unsigned int)

**Note** : getCamera return the camera id


#### simple example

``` lua
camera = getCamera()
if Input.get_key_held(Key.Q_KEY_W) then
		cameraZoomIn(camera)
	end
	
	if Input.get_key_held(Key.Q_KEY_S) then
		cameraZoomOut(camera)
	end
	
	updateRotation(camera, vec3(0,0.5,0)) --rotate the camera
```

### Collision

Function | Argument1 | Argument2
---------| --------- | ---------
getNumCollidedEntities  | current_entity_id(unsigned int) | /
getCollidedEntity       | current_entity_id(unsigned int) | collisionIndex(unsigned int)

**Note** : current_entity_id is passed from C++ side, so just keep it as `thiz`. If you create a new entity in script, then just use the new entity id


#### simple example

``` lua
num_collided_ent =  getNumCollidedEntities(thiz)  --total number
--print(num_collided_ent)
for i = 0, num_collided_ent-1 do 
	collided_entity = getCollidedEntity(thiz, i)
	--print("ent id : " .. collided_entity)
end
```
