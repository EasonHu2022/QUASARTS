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
#include "ResourceManager/FileResource.h"
#include "ResourceManager/ModelResource.h"
#include "ResourceManager/ResourceManager.h"

/*
	entity
*/
#include "Core/EntityWorld.h"
#include "ECS/ECSManager.h"

/*
	Scripts
*/
#include "ECS/System/ScriptSystem.h"

/*
	Audio
*/
#include "ECS/System/AudioSystem.h"


/*
	Resource Loader
*/