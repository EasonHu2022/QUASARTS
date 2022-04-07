#pragma once
//wrap all engine api

#include "Core/Application.h"

/*
	Event Manager
*/


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
//#include "ECS/ECS-Test-Function.h"


/*
	Scripts
*/
#include "Scripts/ScriptsSystem.h"