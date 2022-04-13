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
#include "Render/RendererTemp.h"
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
#include "ECS/System/ExampleSystem.h"


/*
	Scripts
*/
#include "Scripts/ScriptsSystem.h"