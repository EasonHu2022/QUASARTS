# Script System
For Quasarts Engine, we select lua as our scripting language

## How to bind and load a script
1. create an entity first
2. click the entity which you want to bind a script component in Asset Hierarchy window
3. bind: select the menu bar: **Assets -> Add Script Component** and choose the lua file you want to use
4. load and stop: select the menu bar: **Edit -> Play/Stop**

## Update tracking

To keep tracking the content in script, please put everything you want to update in `onUpdate` function
``` lua
--Update the script here
--thiz: current entity id
function onUpdate(thiz)
  --put everything you want to update every frame
end
```


## Current Script API
- Input
- Math
- Audio
- Log
- Transform

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
m3 = (3) 
-- or you can set every value of the matrix
m3 = (1,2,3,4,5,6,7,8,9)
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

### Transform

Function | Argument1 | Argument2
---------| --------- | ---------
entity.x   | current_entity_id(int) | step(float)
entity.y   | current_entity_id(int) | step(float)
entity.z   | current_entity_id(int) | step(float)

**Note** : current_entity_id is passed from C++ side, so just keep it as `thiz`

#### simple example
``` lua
entity.x(thiz, 2)
```














