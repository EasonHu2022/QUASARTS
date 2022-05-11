# Script System
For Quasarts Engine, we select lua as our scripting language

## How to bind and load a script
1. create an entity first
2. click the entity which you want to bind a script component in Asset Hierarchy window
3. bind: select the menu bar: **Assets -> Add Script Component** and choose the lua file you want to use
4. load and stop: select the menu bar: **Edit -> Play/Stop**

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
- Components
  - Transform
  - Health 
- Time

### Input 

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

#### [keycodes and mousecodes list](./InputExporter.cpp)

#### simple example
``` lua
if Input.get_key_held(Key.Q_KEY_A) then
    --do what you want to do here
end
```

``` lua
if Input.get_mouse_held(Mouse.Q_MOUSE_LEFT) then
    --do what you want to do here
end
```

### Math(from glm library)

Quasarts Engine now supports:
- vec2
- vec3
- vec4
- mat3
- mat4


#### simple example
``` lua
-- create a variable
v2 = vec2(2,3)

-- get a diagonal matrix
m3 = mat3(3) 
-- or you can set every value of the matrix
m3 = mat3(1,2,3,4,5,6,7,8,9)
```

### Audio

Quasarts Engine now supports playing sound clip(***.ogg format only***) and long track (***.wav format only***)

Function | Argument
---------| ---------
Audio.playClip    | source name(string)
Audio.pauseClip   | /
Audio.stopClip    | /
Audio.resumeClip  | /
Audio.playTrack   | source name(string)
Audio.pauseTrack  | /
Audio.stopTrack   | /
Audio.resumeTrack | /


Function | Arguments | Description
---------| --------- | ---------
Device.setAttunation     |  /                            | Set distance attenuation model
Device.setPosition3f     |  x, y, z (float)              | Set device position with separate x, y, z
Device.setPositionv      |  vec3                         | Set device position with vec3
Device.setOritentation   |  atX, atY, atZ, upX, upY, upZ | Set device oritentation


### Log

Quasarts Engine now supports log functions

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


#### simple example
``` lua
Qlog("log")
v = 2
Qwarn("v = " .. v)
```

### Components

Quasarts Engine can manipulate other components which belong to current entity

#### Transform

Function | Argument1 | Argument2
---------| --------- | ---------
entity.x   | current_entity_id(unsigned int) | step(float)
entity.y   | current_entity_id(unsigned int) | step(float)
entity.z   | current_entity_id(unsigned int) | step(float)


**Note** : current_entity_id is passed from C++ side, so just keep it as `thiz`


##### simple example
``` lua
entity.x(thiz, 2)
```


Function | Argument1 | Argument2
---------| --------- | ---------
entity.updatePosition    | current_entity_id(unsigned int) | step(vec3)
entity.updateRotation    | current_entity_id(unsigned int) | step(vec3)
entity.updateScale       | current_entity_id(unsigned int) | step(vec3)


**Note** : current_entity_id is passed from C++ side, so just keep it as `thiz`


##### simple example
``` lua
local dP = vec3(0,0,0)
local dR = vec3(0,0,0)
local dS = vec3(0,0,0)
entity.updatePosition(thiz, dP)
entity.updateRotation(thiz, dR)
entity.updateScale(thiz, dS)
```


#### Health

Function | Argument1 | Argument2
---------| --------- | ---------
entity.setMaxHealth       | current_entity_id(unsigned int) | max_health(float)
entity.setCurrentHealth   | current_entity_id(unsigned int) | current_health(float)
entity.getCurrentHealth   | current_entity_id(unsigned int) | /

**Note** : current_entity_id is passed from C++ side, so just keep it as `thiz`


##### simple example
``` lua
init_health = 100
entity.setMaxHealth(thiz, init_health)
entity.setsetCurrentHealth(thiz, init_health)
current_health = entity.getCurrentHealth(thiz)
current_health = current_health - 20 -- change the health
entity.setsetCurrentHealth(thiz, current_health)
```
