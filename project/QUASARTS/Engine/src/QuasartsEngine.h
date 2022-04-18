#pragma once
//wrap all engine api

#include "Core/Application.h"

/*
	Event Manager
*/
#include "Event/EventModule.h"


/*
	Physics Manager
*/
#include "Scene/PhysicsSystem.h"


/*
	Renderer
*/
#include "Render/Renderer.h"
#include "Logger/LogModule.h"
#include "Gui/GuiWrapper.h"

/*
	ResourceMgr
*/

#include "ResourceManager/FileModule.h"
#include "ResourceManager/ModelResource.h"

/*
	entity
*/
#include "Core/EntityWorld.h"
#include "ECS/ECSManager.h"


/*
	Scripts
*/
#include "Scripts/ScriptsSystem.h"

/*
	Audio
*/
#include "Audio/AudioSystem.h"